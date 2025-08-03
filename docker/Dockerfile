FROM archlinux

# Installation des outils de base
RUN pacman -Syu --noconfirm \
    && pacman -S --noconfirm base-devel git xorg-server xorg-xauth xorg-xhost
    
# Installation de Qt Creator
RUN pacman -S --noconfirm qtcreator libgl mesa 

# For libfuzzy
RUN pacman -S --noconfirm ssdeep

# not sure if needed
RUN pacman -S --noconfirm qt5-base

# App directory
RUN mkdir /app

# Copy app dir 
COPY app/ /app

# Define working dir
WORKDIR /app

# run
CMD ["./lancer.sh"]