#include <iostream>
#include <windows.h>

using namespace std;


// выводим поле
void show_field(char field[10][10])
{
    for(int i = 0; i<10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            cout<<field[i][j]<<" ";
        }
        cout<<endl;
    }
}


// возвращает 1 если фигура под индексом - это фигура противника, 0 - если фигура союзника и 2 если там пусто
int is_enemy(int start_x, int start_y, int  end_x, int end_y, char field[10][10])
{
    if(field[end_y + 1][end_x + 1] == '_') return 2;


    // фигуры, имеющие заглавный символ, принадлежат игроку снизу
    if(64 < int(field[start_y + 1][start_x + 1]) and int(field[start_y + 1][start_x + 1]) < 91)
    {
        if(64 < int(field[end_y + 1][end_x + 1]) and int(field[end_y + 1][end_x + 1]) < 91)
            return 0;
        else
            return 1;
    }
    else
    {
        if(64 < int(field[end_y + 1][end_x + 1]) and int(field[end_y + 1][end_x + 1]) < 91)
            return 1;
        else
            return 0;
    }
}


// проверяет валидность движения пешки по y
bool is_valid_P_motion(int start_x, int start_y, int  end_x, int end_y, char field[10][10])
{
    // проверяем наличие на конечной координате врага
    if(is_enemy(start_x, start_y, end_x, end_y, field) == 1)
    {
        // враг есть

        if(start_x == end_x) return false;
        else
        {
            // враг находится слева/справа сверху
            if(start_x - end_x == 1 or start_x - end_x == -1)
            {
                // проверим валидность по оси Y


                // смотрим ситуацию для нижнего игрока
                if(field[start_y+1][start_x+1] == 'P')
                {
                    if(end_y - start_y == -1) return true;
                    else
                    {
                        // если пешка в начальной координате, то она может сходить на 2 клетки
                        if(end_y - start_y == -2 and start_y == 7) return true;
                        else return false;
                    }
                }
                // смотрим ситуацию для верхнего игрока
                else
                {
                    if(end_y - start_y == 1) return true;
                    else
                    {
                        // если пешка в начальной позиции, то можно сходить на 2 клетки
                        if(end_y - start_y == 2 and start_y == 2) return true;
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
        if(is_enemy(start_x, start_y, end_x, end_y, field) == 0) return false;
        // перед нами пустая клетка
        else
        {
            if(start_x == end_x)
            {
                // проверяем валидность по оси Y

                // смотрим ситуацию для нижнего игрока
                if(field[start_y+1][start_x+1] == 'P')
                {
                    if(end_y - start_y == -1) return true;
                    else
                    {
                        // если пешка в начальной координате, то она может сходить на 2 клетки
                        if(end_y - start_y == -2 and start_y == 7) return true;
                        else return false;
                    }
                }
                // смотрим ситуацию для верхнего игрока
                else
                {
                    if(end_y - start_y == 1) return true;
                    else
                    {
                        // если пешка в начальной позиции, то можно сходить на 2 клетки
                        if(end_y - start_y == 2 and start_y == 2) return true;
                        else return false;
                    }
                }
                //return true;
            }
            else return false;
        }
    }
}


// возвращает true если преград в данном отрезке нет, смотрит по оси x
bool barrier_x(int start_x, int  end_x, int y, char field[10][10])
{
    // движение в правую сторону
    if (start_x < end_x)
    {
        for(int i = start_x+2; i<end_x+1; i++)
        {
            if(field[y+1][i] != '_') return false;
        }
        return true;
    }
    // движение в левую сторону
    else
    {
        for(int i = start_x; i>end_x+1; i--)
        {
            if(field[y+1][i] != '_') return false;
        }
        return true;
    }
}


// возвращает true если преград в данном отрезке нет, смотрит по оси y
bool barrier_y(int start_y, int  end_y, int x, char field[10][10])
{
    // движение в правую сторону
    if (start_y < end_y)
    {
        for(int i = start_y+2; i<end_y; i++)
        {
            if(field[i][x+1] != '_') return false;
        }
        return true;
    }
    // движение в левую сторону
    else
    {
        for(int i = start_y; i>end_y; i--)
        {
            if(field[i][x+1] != '_') return false;
        }
        return true;
    }
}

// проверяем валидность движения туры (ходит вертикально или горизонтально)
bool is_valid_R_motion(int start_x, int start_y, int  end_x, int end_y, char field[10][10])
{
    if(is_enemy(start_x, start_y, end_x, end_y, field) == 1)
    {
        // враг в конечной точке

        // проверяем наличие препятствий до врага по оси X
        if(start_x == end_x)
        {
            if(barrier_y(start_y, end_y, start_x, field)) return true;
        }

        else
        {
            // проверяем наличие препятствий по оси Y
            if(start_y == end_y)
            {
                if(barrier_x(start_x, end_x, start_y, field)) return true;
            }
            else return false;
        }
    }
    else
    {
        // врага нет в конечной точке

        // союзник в клетке
        if(is_enemy(start_x, start_y, end_x, end_y, field) == 0) return false;
        else
        {
            // пустая клетка

            // проверяем наличие препятствий по оси X если конечная координата по X равна конечной
            if(start_x == end_x)
            {
                if (barrier_y(start_y, end_y, start_x, field)) return true;
            }
            else
            {
                // проверяем наличие препятствий по оси Y
                if(start_y == end_y)
                {
                    if(barrier_x(start_x, end_x, start_y, field)) return true;
                }
                else return false;
            }
        }
    }
}


void P_motion(int start_x, int start_y, int  end_x, int end_y, char field[10][10])
{
    if(is_valid_P_motion(start_x, start_y, end_x, end_y, field))
    {
        field[end_y + 1][end_x + 1] = field[start_y + 1][start_x + 1];
        field[start_y + 1][start_x + 1] = '_';
    }
    else cout<<"You write not correct data";
}


void motion(int start_x, int start_y, int  end_x, int end_y, char field[10][10])
{
    // если была выбрана пустая клетка, а не фигура
    if(field[start_y + 1][start_x + 1] == '_' or start_x < 1 or start_y < 1)
        cout<<"You write not correct data";
    else
    {
        // пешка
        if(field[start_y + 1][start_x + 1] == 'P' or field[start_y + 1][start_x + 1] == 'p')
        {
            if(is_valid_P_motion(start_x, start_y, end_x, end_y, field))
            {
                field[end_y + 1][end_x + 1] = field[start_y + 1][start_x + 1];
                field[start_y + 1][start_x + 1] = '_';
            }
            else cout<<"You write not correct data";
        }
        else
        {
            // тура
            if(field[start_y + 1][start_x + 1] == 'R' or field[start_y + 1][start_x + 1] == 'r')
            {
                if(is_valid_R_motion(start_x, start_y, end_x, end_y, field))
                {
                    field[end_y + 1][end_x + 1] = field[start_y + 1][start_x + 1];
                    field[start_y + 1][start_x + 1] = '_';
                }
                else cout<<"You write not correct data";
            }
            else
            {
                cout<<start_x<<"  "<<start_y<<"   "<<field[start_y + 1][start_x + 1]<<endl;
                field[end_y + 1][end_x + 1] = field[start_y + 1][start_x + 1];
                field[start_y + 1][start_x + 1] = '_';
            }

        }
    }
}


int main()
{
    char field[10][10]{
        '#', '|', '1', '2', '3', '4', '5', '6', '7', '8',
        '-', '+', '-', '-', '-', '-', '-', '-', '-', '-',
        '1','|', 'r', 'h', 'e', 'q', 'k', 'e', 'h', 'r',
        '2','|', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',
        '3','|', '_', '_', '_', '_', '_', '_', '_', '_',
        '4','|', '_', '_', '_', '_', '_', '_', '_', '_',
        '5','|', '_', '_', '_', '_', '_', '_', '_', '_',
        '6','|', '_', '_', '_', '_', '_', '_', '_', '_',
        '7','|','P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
        '8','|', 'R', 'H', 'E', 'Q', 'k', 'E', 'H', 'R',
    };

    /*
        P - это пешка
        R - это ладья
        H - это конь
        E - слон
        Q - королева
        K - король
    */

    int start_x, start_y, end_x, end_y, i=0;

    while(true)
    {
        show_field(field);
        cin>>start_x>>start_y>>end_x>>end_y;
        motion(start_x, start_y, end_x, end_y, field);

        i++;
        Sleep(1000);
        system("cls");
    }

    system("PAUSE >> VOID");
    return 0;
}
