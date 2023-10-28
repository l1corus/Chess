#include <iostream>
#include <windows.h>

using namespace std;


// возвращает кол-во очков за шахматную фигуру
int points_for_chess_piece(char figure)
{
    switch (figure)
    {
        case 'p':
            return 1;
        case 'P':
            return 1;

        case 'r':
            return 5;
        case 'R':
            return 5;

        case 'h':
            return 3;
        case 'H':
            return 3;

        case 'e':
            return 3;
        case 'E':
            return 3;
        
        case 'q':
            return 9;
        case 'Q':
            return 9;
    }
    return 0;
}


// выводим поле, номер хода, кол-во очков 
void show_field(char field[10][10], int i)
{
    cout << "Motion number: \t" << i + 1 << "\n";
    if (i % 2 == 1) cout << "Action: lower letters\n\n";
    else cout << "Action: upper letters\n\n";

    int upper_letters_points = 39, lower_letters_points = 39;
    bool upper_letters_king = false, lower_letters_king = false;


    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            cout << field[i][j] << " ";

            if (64 < int(field[i][j]) and int(field[i][j]) < 91) upper_letters_points -= points_for_chess_piece(field[i][j]);
            if (96 < int(field[i][j]) and int(field[i][j]) < 123) lower_letters_points -= points_for_chess_piece(field[i][j]);
            if (field[i][j] == 'K') upper_letters_king = true;
            if (field[i][j] == 'k') lower_letters_king = true;

        }
        cout << endl;
    }

    cout << "\nLower letters points:\t" << lower_letters_points;
    cout << "\nUpper letters point:\t" << upper_letters_points<<endl<<endl;

    if (lower_letters_king == false) cout << "\n\n\t\t\tUPPER LETTERS WIN!!!";
    if (upper_letters_king == false) cout << "\n\n\t\t\tLOWER LETTERS WIN!!!";

}


// возвращает 1 если фигура под индексом - это фигура противника, 0 - если фигура союзника и 2 если там пусто
int is_enemy(int start_x, int start_y, int  end_x, int end_y, char field[10][10])
{
    if (field[end_y + 1][end_x + 1] == '_') return 2;


    // фигуры, имеющие заглавный символ, принадлежат игроку снизу
    if (64 < int(field[start_y + 1][start_x + 1]) and int(field[start_y + 1][start_x + 1]) < 91)
    {
        if (64 < int(field[end_y + 1][end_x + 1]) and int(field[end_y + 1][end_x + 1]) < 91)
            return 0;
        else
            return 1;
    }
    else
    {
        if (64 < int(field[end_y + 1][end_x + 1]) and int(field[end_y + 1][end_x + 1]) < 91)
            return 1;
        else
            return 0;
    }
}


// возвращает true если преград в данном отрезке нет, смотрит по оси x
bool barrier_x(int start_x, int  end_x, int y, char field[10][10])
{
    // движение в правую сторону
    if (start_x < end_x)
    {
        for (int i = start_x + 2; i < end_x + 1; i++)
        {
            if (field[y + 1][i] != '_') return false;
        }
        return true;
    }
    // движение в левую сторону
    else
    {
        for (int i = start_x; i > end_x + 1; i--)
        {
            if (field[y + 1][i] != '_') return false;
        }
        return true;
    }
}


// возвращает true если преград в данном отрезке нет, смотрит по оси y
bool barrier_y(int start_y, int  end_y, int x, char field[10][10])
{
    // движение вниз
    if (start_y < end_y)
    {
        for (int i = start_y + 1 + 1; i < end_y - 1; i++)
        {
            if (field[i][x + 1] != '_') return false;
        }
        return true;
    }
    // движение вверх
    else
    {
        for (int i = start_y + 1 - 1; i > end_y + 1; i--)
        {
            if (field[i][x + 1] != '_') return false;
        }
        return true;
    }
}


// проверяет можно ли из начальной точки попасть в конечную по диагонали
bool diagonal(int start_x, int start_y, int end_x, int end_y)
{
    if (abs(start_x - end_x) == abs(start_y - end_y)) return true;
    else return false;
}


// ищет препятствия по диагонали в право вверх
bool barrier_right_diagonal(int start_x, int start_y, int end_x, int end_y, char field[10][10])
{
    int x = start_x + 2;
    for (int y = start_y - 1; y > end_y; y--)
    {
        if (field[y][x] != '_' and is_enemy(start_x, start_y, x, y, field) != 1) return false;
        x++;
    }
    return true;
}


// ищет препятствия по диагонали в лево вверх
bool barrier_left_diagonal(int start_x, int start_y, int end_x, int end_y, char field[10][10])
{
    int x = start_x;
    for (int y = start_y - 1; y > end_y; y--)
    {
        if (field[y][x] != '_' and is_enemy(start_x, start_y, x, y, field) != 1) return false;
        x--;
    }
    return true;
}


 // ищет препятствия по диагонали вниз в парвую сторону 
bool barrier_right_down_diagonal(int start_x, int start_y, int end_x, int end_y, char field[10][10])
{
    int x = start_x + 2;
    for (int y = start_y + 2; y <= end_y; y++)
    {
        if (field[y][x] != '_' and is_enemy(start_x, start_y, x, y, field) != 1) return false;
        x++;
    }
    return true;
}


// ищет препятствия по диагонали вниз в левую сторону
bool barrier_left_down_diagonal(int start_x, int start_y, int end_x, int end_y, char field[10][10])
{
    int x = start_x;
    for (int y = start_y + 2; y <= end_y; y++)
    {
        if (field[y][x] != '_' and is_enemy(start_x, start_y, x, y, field) != 1) return false;
        x--;
    }

    return true;
}


// проверяет валидность движения пешки
bool is_valid_P_motion(int start_x, int start_y, int  end_x, int end_y, char field[10][10])
{
    // проверяем наличие на конечной координате врага
    if (is_enemy(start_x, start_y, end_x, end_y, field) == 1)
    {
        // враг есть

        if (start_x == end_x) return false;
        else
        {
            // враг находится слева/справа сверху
            if (start_x - end_x == 1 or start_x - end_x == -1)
            {
                // проверим валидность по оси Y


                // смотрим ситуацию для нижнего игрока
                if (field[start_y + 1][start_x + 1] == 'P')
                {
                    if (end_y - start_y == -1) return true;
                    else
                    {
                        // если пешка в начальной координате, то она может сходить на 2 клетки
                        if (end_y - start_y == -2 and start_y == 7) return true;
                        else return false;
                    }
                }
                // смотрим ситуацию для верхнего игрока
                else
                {
                    if (end_y - start_y == 1) return true;
                    else
                    {
                        // если пешка в начальной позиции, то можно сходить на 2 клетки
                        if (end_y - start_y == 2 and start_y == 2) return true;
                        else return false;
                    }
                }

            }
            //else return false;
        }
    }
    else
    {
        // врага на конечной координате нет

        // проверяем наличие союзника на конечной координате
        if (is_enemy(start_x, start_y, end_x, end_y, field) == 0) return false;
        // перед нами пустая клетка
        else
        {
            if (start_x == end_x)
            {
                // проверяем валидность по оси Y

                // смотрим ситуацию для нижнего игрока
                if (field[start_y + 1][start_x + 1] == 'P')
                {
                    if (end_y - start_y == -1) return true;
                    else
                    {
                        // если пешка в начальной координате, то она может сходить на 2 клетки
                        if (end_y - start_y == -2 and start_y == 7) return true;
                        else return false;
                    }
                }
                // смотрим ситуацию для верхнего игрока
                else
                {
                    if (end_y - start_y == 1) return true;
                    else
                    {
                        // если пешка в начальной позиции, то можно сходить на 2 клетки
                        if (end_y - start_y == 2 and start_y == 2) return true;
                        else return false;
                    }
                }
                //return true;
            }
            else return false;
        }
    }
}


// проверяем валидность движения туры (ходит вертикально или горизонтально)
bool is_valid_R_motion(int start_x, int start_y, int  end_x, int end_y, char field[10][10])
{
    if (is_enemy(start_x, start_y, end_x, end_y, field) == 1)
    {
        // враг в конечной точке

        // проверяем наличие препятствий до врага по оси X
        if (start_x == end_x)
        {
            if (barrier_y(start_y, end_y, start_x, field)) return true;
        }

        else
        {
            // проверяем наличие препятствий по оси Y
            if (start_y == end_y)
            {
                if (barrier_x(start_x, end_x, start_y, field)) return true;
            }
            else return false;
        }
    }
    else
    {
        // врага нет в конечной точке

        // союзник в клетке
        if (is_enemy(start_x, start_y, end_x, end_y, field) == 0) return false;
        else
        {
            // пустая клетка

            // проверяем наличие препятствий по оси X если конечная координата по X равна конечной
            if (start_x == end_x)
            {
                if (barrier_y(start_y, end_y, start_x, field)) return true;
            }
            else
            {
                // проверяем наличие препятствий по оси Y
                if (start_y == end_y)
                {
                    if (barrier_x(start_x, end_x, start_y, field)) return true;
                }
                else return false;
            }
        }
    }
}


// проверка на валидность хода коня
bool is_valid_H_motion(int start_x, int start_y, int  end_x, int end_y, char field[10][10])
{
    if(start_x + 1 == end_x or start_x - 1 == end_x)
    {
        if ((start_y - 2 == end_y or start_y + 2 == end_y) and is_enemy(start_x, start_y, end_x, end_y, field) != 0) return true;
        else return false;
    }

    if (start_x + 2 == end_x or start_x - 2 == end_x)
    {
        if ((start_y + 1 == end_y or start_y - 1 == end_y) and is_enemy(start_x, start_y, end_x, end_y, field) != 0) return true;
        else return false;
    }
    return false;
}


// проверка на валидность хода короля
bool is_valid_K_motion(int start_x, int start_y, int  end_x, int end_y, char field[10][10])
{
    if (start_x + 1 == end_x or start_x - 1 == end_x or start_x == end_x)
    {
        if ((start_y + 1 == end_y or start_y - 1 == end_y or start_y == end_y) and is_enemy(start_x, start_y, end_x, end_y, field) != 0) return true;
        else return false;
    }
}


// проверка на валидность хода слона (ходит по диагоналям)
bool is_valid_E_motion(int start_x, int start_y, int end_x, int end_y, char field[10][10])
{
    if (start_x == end_x or start_y == end_y or diagonal(start_x, start_y, end_x, end_y) == false) return false;

    if (start_x > end_x)
    {
        // диагонали верхней полуплоскости
        if (start_y > end_y)
        {
            if (barrier_left_diagonal(start_x, start_y, end_x, end_y, field))
                return true;
            else return false;
        }  
        else 
        {
            if (barrier_left_down_diagonal(start_x, start_y, end_x, end_y, field))
                return true;
            else return false;
        }
                    
    }
    else
    {
        // диагонали нижней полуплоскости
        if (start_y > end_y)
        {
            if (barrier_right_diagonal(start_x, start_y, end_x, end_y, field))
                return true;
            else return false;
        }
            
        else
        {
            if (barrier_right_down_diagonal(start_x, start_y, end_x, end_y, field))
                return true;
            else return false; 
        }
                
    }
    return false;
}



// проверка на валидность хода Королевы (ходит по диагоналям, вертикали и горизонтали)
bool is_valid_Q_motion(int start_x, int start_y, int end_x, int end_y, char field[10][10])
{
    // если движение по вертикали или горизонтали
    if (start_x == end_x or start_y == end_y)
    {
        return is_valid_R_motion(start_x, start_y, end_x, end_y, field);
    }
    // движение по диагоналям
    else
    {
        return is_valid_E_motion(start_x, start_y, end_x, end_y, field);
    }
}


// функция, которая просто передвигает фигуры в конечную точку (вызывается если фигура делает валидный ход)
void mot(int start_x, int start_y, int  end_x, int end_y, char field[10][10])
{
    field[end_y + 1][end_x + 1] = field[start_y + 1][start_x + 1];
    field[start_y + 1][start_x + 1] = '_';
}


// определяет фигуру и проверяет валидность её передвижения, если фигура не может так сходить, то возвращает false, иначе true
// i - это номер хода (если i чётное, то должны ходить заглавные(нижние фигуры))
bool Motion(int start_x, int start_y, int  end_x, int end_y, char field[10][10], int i)
{
    if (start_x == end_x and start_y == end_y)
    {
        cout << "You have entered the same coordinates of the start and end point\n";
        return false;
    }


    switch (field[start_y + 1][start_x + 1])
    {
        case 'p':
            if (i % 2 == 0) 
            {
                cout << "It's not your turn now\n"; 
                return false;
            } 
            if (is_valid_P_motion(start_x, start_y, end_x, end_y, field))
            {
                mot(start_x, start_y, end_x, end_y, field);
                return true;
            }    
            else
            {
                cout << "You write not correct data\n";
                return false;
            }
        case 'P':
            if (i % 2 == 1)
            {
                cout << "It's not your turn now\n";
                return false;
            }
            if (is_valid_P_motion(start_x, start_y, end_x, end_y, field))
            {
                mot(start_x, start_y, end_x, end_y, field);
                return true;
            }
            else
            {
                cout << "You write not correct data\n";
                return false;
            }


        case 'r':
            if (i % 2 == 0)
            {
                cout << "It's not your turn now\n";
                return false;
            }
            if (is_valid_R_motion(start_x, start_y, end_x, end_y, field))
            {
                mot(start_x, start_y, end_x, end_y, field);
                return true;
            }
            else
            {
                cout << "You write not correct data\n";
                return false;
            }
        case 'R':
            if (i % 2 == 1)
            {
                cout << "It's not your turn now\n";
                return false;
            }
            if (is_valid_R_motion(start_x, start_y, end_x, end_y, field))
            {
                mot(start_x, start_y, end_x, end_y, field);
                return true;
            }
            else
            {
                cout << "You write not correct data\n";
                return false;
            }


        case 'h':
            if (i % 2 == 0)
            {
                cout << "It's not your turn now\n";
                return false;
            }
            if (is_valid_H_motion(start_x, start_y, end_x, end_y, field))
            {
                mot(start_x, start_y, end_x, end_y, field);
                return true;
            }
            else
            {
                cout << "You write not correct data\n";
                return false;
            }
        case 'H':
            if (i % 2 == 1)
            {
                cout << "It's not your turn now\n";
                return false;
            }
            if (is_valid_H_motion(start_x, start_y, end_x, end_y, field))
            {
                mot(start_x, start_y, end_x, end_y, field);
                return true;
            }
            else
            {
                cout << "You write not correct data\n";
                return false;
            }


        case 'k':
            if (i % 2 == 0)
            {
                cout << "It's not your turn now\n";
                return false;
            }
            if (is_valid_K_motion(start_x, start_y, end_x, end_y, field))
            {
                mot(start_x, start_y, end_x, end_y, field);
                return true;
            }
            else
            {
                cout << "You write not correct data\n";
                return false;
            }
        case 'K':
            if (i % 2 == 1)
            {
                cout << "It's not your turn now\n";
                return false;
            }
            if (is_valid_K_motion(start_x, start_y, end_x, end_y, field))
            {
                mot(start_x, start_y, end_x, end_y, field);
                return true;
            }
            else
            {
                cout << "You write not correct data\n";
                return false;
            }

        case 'e':
            if (i % 2 == 0)
            {
                cout << "It's not your turn now\n";
                return false;
            }
            if (is_valid_E_motion(start_x, start_y, end_x, end_y, field))
            {
                mot(start_x, start_y, end_x, end_y, field);
                return true;
            }
            else
            {
                cout << "You write not correct data\n";
                return false;
            }
        case 'E':
            if (i % 2 == 1)
            {
                cout << "It's not your turn now\n";
                return false;
            }
            if (is_valid_E_motion(start_x, start_y, end_x, end_y, field))
            {
                mot(start_x, start_y, end_x, end_y, field);
                return true;
            }
            else
            {
                cout << "You write not correct data\n";
                return false;
            }


        case 'q':
            if (i % 2 == 0)
            {
                cout << "It's not your turn now\n";
                return false;
            }
            if (is_valid_Q_motion(start_x, start_y, end_x, end_y, field))
            {
                mot(start_x, start_y, end_x, end_y, field);
                return true;
            }
            else
            {
                cout << "You write not correct data\n";
                return false;
            }
        case 'Q':
            if (i % 2 == 1)
            {
                cout << "It's not your turn now\n";
                return false;
            }
            if (is_valid_Q_motion(start_x, start_y, end_x, end_y, field))
            {
                mot(start_x, start_y, end_x, end_y, field);
                return true;
            }
            else
            {
                cout << "You write not correct data\n";
                return false;
            }

        case '_':
            cout << "There is no figure in this position\n";
            return false;
    }
    return false;
}


int main()
{
    char field[10][10]{
        '#', '|', '1', '2', '3', '4', '5', '6', '7', '8',
        '-', '+','-', '-', '-', '-', '-', '-', '-', '-',
        '1','|', 'r', 'h', 'e', 'q', 'k', 'e', 'h', 'r',
        '2','|', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',
        '3','|', '_', '_', '_', '_', '_', '_', '_', '_',
        '4','|', '_', '_', '_', '_', '_', '_', '_', '_',
        '5','|', '_', '_', '_', '_', '_', '_', '_', '_',
        '6','|', '_', '_', '_', '_', '_', '_', '_', '_',
        '7','|', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
        '8','|', 'R', 'H', 'E', 'Q', 'K', 'E', 'H', 'R',
    };

    /*
        P - это пешка 
        R - это ладья 
        H - это конь 
        E - слон 
        Q - королева
        K - король 
    */

    int start_x, start_y, end_x, end_y, i = 0;
    char figure;

    while (true)
    {
        show_field(field, i);

        while(true)
        {
            cin >> start_x >> start_y >> end_x >> end_y;
            // проверка сходил ли игрок
            if (Motion(start_x, start_y, end_x, end_y, field, i))
                break; 
        }

        // если пешка дошла до края доски, то даём возможность сменить фигуру на коня/слона/туру/королеву
        if ((field[end_y + 1][end_x + 1] == 'P' or field[start_y + 1][start_x + 1] == 'p') and (end_y == 1 or end_y == 8))
        {
            cout << "Choose a chess figure (horse / elephant / rook / queen)" << endl;
            while (true)
            {
                cin >> figure;
                if ((field[end_y + 1][end_x + 1] == 'P') and (figure == 'H' or figure == 'E' or figure == 'R' or figure == 'Q')) break;
                if ((field[end_y + 1][end_x + 1] == 'p') and (figure == 'h' or figure == 'e' or figure == 'r' or figure == 'q')) break;
                
                cout << "You choose not valid figure\n";
            }
            
            field[end_y + 1][end_x + 1] = figure;
        }
        
        // i - это номер хода
        i++;
        Sleep(1000);
        system("cls");
    }

    system("PAUSE >> VOID");
    return 0;
}
