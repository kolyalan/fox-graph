# Простая Roguelike игра, вариант А1

Карта лабиринта задается в текстовых файлах в директории resources/levels, уровни запускаются в лексикографическом порядке соответствующих файлов. 
- В первой строке уровня задается его размер в формате WxH, где W - ширина уровня, H - высота. Далее идет карта уровня.
- '.' - пол
- '#' - стены, должны образовывать замкнутые многоугольники.
- ' ' - пустота. Заполняет пространство за стенами, недоступное игроку и образует провалы в полу посреди доступного пространства.
- 'T' - ловушка-шипы
- '@' - начальное положение игрока на уровне. Уровень должен содержать ровно один такой символ.
- 'x' - выход с уровня. Переход на следующий либо конец игры, если уровень последний. Должны встречаться парами в самой верхней либо боковых стенах

Лабиринт отображается тайлами, причем некоторые виды стен занимают два тайла. 

Игрок двигается с помощью WASD, может прыгать нажатием на пробел. Прыжком можно перепрыгнуть одну клетку, прямо либо по диагонали.

При попадании в пустоту или клетку с шипами игрок умирает и может начать игру с первого уровня. Сообщение о смерти выводится графикой с плавным затеменением фона. Шипы и пустоту можно перепрыгнуть.

Переход на следующий уровень сопровождается затемнением старой карты и постепенным проявлением новой (fade-out/fade-in). Если уровень последний выводится сообщение о победе с затемнением карты.

Присутствует спрайтовая анимация персонажа стоящего на месте, в движении, прыжке, анимация смерти.

Реализован большой лабиринт, карта уровня существенно больше области видимости, видимая область перемещается вместе с игроком.

Видео с записью геймплея прилагается.

### Запуск:
    cmake .
    make
    cd bin
    ./main