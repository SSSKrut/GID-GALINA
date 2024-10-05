# Guid Galina | GID Galina

School project in cooperation with [Politech University of Saint-Petersburg](https://www.spbstu.ru/)
![SPBSTU](https://www.spbstu.ru/local/templates/main/img/favicon-16x16.png).

The project was made by me, Nalobin Alexander, and four of my friends: Daniil Zykov, Anastasia Barinova, Konstantin Kuzmin and Evgeny Isaenko in 2021. 

Thank you guys, for the school years;)

> [!NOTE]
> If you are the copyright holder of any information or media in this project, please let me know in any way you can and I will remove it.

### Goals of the project

- Gain knowledge of web and frontend disciplines.

- Learn to work in a team and distribute tasks.

- Learn how to layout articles.

- Gain experience in creating user interface.

- Learn how to make a backend.

### Start this project

You need to compile the `main.cpp`, you also need sfml:

- ```sudo pacman -S sfml``` for arch linux
- ```g++ -o main src/main.cpp -lsfml-network -lsfml-system```
- exec ```./main``` from `~/GID-GALINA/`, remember that the backend server only reads and sends files from this folder, otherwise you need to change prefix name of the home-folder of `index.html` in `main.cpp` from "Galina" to "your-folder-name" and then recompile.

The default port for the server is 7860.

#### Note

The technical implementation contains a lot of problems in all places and I'm fully aware of that. However, as a school project it still seems to me to be a good achievement. 