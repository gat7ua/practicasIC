# Detector de aristas Canny 

Using the program is fairly simple. Start by downloading or cloning the repo. I already have two sample images that can be used, however, you can use your own pictures. Just make sure to copy them into the "input_images" folder and that they're in the .pgm format.

Once you have your copy of the program, open the program's directory in a Linux terminal and then type "make canny". That will do all the necessary compilation and produce the "canny.out" executable file.

Last, type the command "./canny.out <name_of_input_image> <high_threshold> <sigma_value>" in your Linux terminal (make sure to not include the angle brackets). A typical value to use as your "high_threshold" would be ~100, and a typical value for "sigma" would be 1.

