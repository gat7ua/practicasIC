#include <vector>
#include <algorithm>
#include "genetic.h"
#include "individual.h"
#include "mlp/mlp.h"
#include "utils/utils.h"
#include <thread>
#include <random>
#include <fstream>
#include <iostream>
#include <functional>
#include <omp.h>

Genetic::Genetic(int population, const std::string &fileName, std::function<Individual*()> createRandomIndividual, Simulation* simulation)
{
    this->population = population;
    this->createRandomIndividual = createRandomIndividual;
    this->simulation = simulation;
    this->fileName = fileName;
    this->simulationStartTime = 0.0;
    this->generation = 1;
}

Genetic::~Genetic()
{
    for(int i = 0; i < individuals.size();i++)
    {
        delete individuals[i];
    }
    
    individuals.clear();
    
    if(simulation != NULL)
    {
        delete simulation;
        simulation = NULL;
    }
    
    std::cout<<"Genetic destroyed"<<std::endl;
}

void Genetic::initialize()
{
    int i = 0;
    if(!load())
    {
        std::cout<<"Initializing..."<<std::endl;
        individuals.clear();
        individuals.resize(population);
        
        omp_set_num_threads(population);
        #pragma omp parallel shared(individuals, i) 
        {
            for(i = 0; i < population; i++){ 
                individuals[i] = createRandomIndividual();
            }    
        }
    }
    this->simulationStartTime = omp_get_wtime();
    simulation->init(individuals);
}

bool Genetic::load()
{
    if(fileName.empty())
    {
        std::cout<<"No generation file specified"<<std::endl;
        
        return false;
    }
    
    std::cout<<"Loading generation file "<<fileName<<std::endl;
        
    std::ifstream ifs(fileName.c_str(), std::ios::binary);
    if(ifs.is_open())
    {
        std::cout<<"Reading previous state..."<<std::endl;
        ifs.read(reinterpret_cast<char*>(&generation), sizeof(generation));
        std::cout<<"Generation "<<generation<<std::endl;
        ifs.read(reinterpret_cast<char*>(&population), sizeof(population));
        std::cout<<"Individuals "<<population<<std::endl;
        individuals.clear();
        individuals.resize(population);
        for(int i =0;i<individuals.size();i++)
        {
            individuals[i] = createRandomIndividual();
            Mlp* mlp = individuals[i]->mlp;
            int numWeights = mlp->getNumWeights();
            std::vector<double> weights(numWeights);
            
            ifs.read(reinterpret_cast<char*>(&weights[0]), sizeof(double)*weights.size());
            mlp->setWeights(weights);
            
            std::vector<bool> connections(numWeights);
            std::vector<char> temp(numWeights);
            ifs.read(reinterpret_cast<char*>(&temp[0]), sizeof(char)*connections.size());
            for(int j=0;j<numWeights;j++)
            {
                connections[j] = temp[j] > 0;
            }
            
            mlp->setConnections(connections);
        }
        ifs.close();
        
        return true;
    }
    
    std::cout<<"Could not open generation file "<<fileName<<std::endl;
    
    return false;
}

void Genetic::save()
{
    if(fileName.empty()) 
        return;
        
    std::ofstream of(fileName.c_str(), std::ios::binary);
    if(of.is_open())
    {
        int individualsSize = individuals.size();
        of.write(reinterpret_cast<char*>(&generation), sizeof generation);
        of.write(reinterpret_cast<char*>(&individualsSize), sizeof individualsSize);
        for(int i = 0;i<individuals.size();i++)
        {
            std::vector<double> weights = individuals[i]->mlp->getWeights();
            of.write(reinterpret_cast<char*>(&weights[0]), sizeof(double)*weights.size());
            std::vector<bool> connections = individuals[i]->mlp->getConnections();
            //of.write(reinterpret_cast<char*>(&connections), sizeof(bool)*connections.size());
            std::copy(connections.begin(), connections.end(), std::ostreambuf_iterator<char>(of));
        }
        of.close();
    }
}

void Genetic::updateAndEvolve()
{
    if(!simulation->run(true))
    {
        if (this->simulationStartTime > 0.0)
        {
            std::cout<<"Total simulation time: "<<omp_get_wtime() - this->simulationStartTime<<std::endl;
        }
        this->simulationStartTime = omp_get_wtime();
        
        std::vector<Individual*> newGeneration = nextGeneration();
        
        for(int i = 0;i < individuals.size(); i++)
        {
            delete individuals[i];
        }
        individuals = newGeneration;
        generation++;
        std::cout<<"Generation "<<generation<<std::endl;
        save();
        
        simulation->init(individuals);
    }
}

// Aquí
std::vector<Individual*> Genetic::nextGeneration()
{
    
    std::vector<Individual*> newGeneration(individuals.size());
    std::vector<Individual*> best = bestIndividuals();
    int chunk = ((best.size() / 2) / 4);
    int contador = 0;
  
    omp_set_num_threads(4);
    #pragma omp parallel shared(newGeneration, best, chunk, contador) 
    {
        #pragma omp for schedule(dynamic, chunk)
        for(int i = 0; i < best.size() / 2; i++){  
            Individual *elite = createRandomIndividual();
           
            elite->mlp->setWeights(best[i]->mlp->getWeights());
            elite->mlp->setConnections(best[i]->mlp->getConnections());
            #pragma omp critical
            {
                newGeneration[i] = elite;
                if(omp_get_thread_num() == 1){ //Codigo que nos sirve para ver que cada hilo ejecuta (best.size()/2)/4 de veces,
                    contador += 1;             // Y asi se dividen el trabajo.
                }
            }

        };
    }
    int chunk2 = (individuals.size() - (best.size() / 2)) / 4;
    // The remaining indiviuals are combination of two random individuals from the best
    //omp_set_num_threads(4);
    #pragma omp parallel shared(newGeneration, best, chunk)
    {
        #pragma omp for schedule(dynamic, chunk2)
        for(int i = best.size() / 2; i < individuals.size(); i++){
            Individual *child = createRandomIndividual();
            int a = randomNumber(0.0, 1.0) * (best.size()-1);
            int b = randomNumber(0.0, 1.0) * (best.size()-1);
            Individual *parent1 = best[a];
            Individual *parent2 = best[b];
            
            parent1->mate(*parent2, child);
            #pragma omp critical
            {
                newGeneration[i] = child;  
            }
        };
    }
    return newGeneration;
}

std::vector<Individual*> Genetic::bestIndividuals()
{
    for(int i=0;i<individuals.size();i++)
        individuals[i]->calculateFitness();

    sort(individuals.begin(), individuals.end(), [](Individual *a, Individual *b)
    {
        return a->fitness > b->fitness;
    });
    
    for(int i = 0;i<10;i++)
        std::cout<<individuals[i]->fitness<<"\t";
    std::cout<<std::endl;
    
    return std::vector<Individual*>(individuals.begin(), individuals.begin() + ((int)(individuals.size()*0.2)));
}

