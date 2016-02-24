

# installing pygame for anaconda

conda install binstar
conda install anaconda-client

# choose the proper install based on the search
# anaconda search -t conda pygame
# 
# This is the install that worked for me
# it should work for 64-bit Ubuntu system.
conda install -c https://conda.binstar.org/tlatorre pygame

# Install PIL (python image library)
conda install pillow

# tornado webserver (for the website)
conda install tornado

# For compiling the C extensions
sudo apt-get install python-dev


