#define array_size 26 /// apparently, I can't do "const int" for fixed array sizes in C, I have to do it this way

#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>


void array_display(char charDisplay[]) /// this handles the the array that is shown to the user
{
    int prev_y, prev_x;
    getyx(stdscr, prev_y, prev_x);

    move(1 , 0);
    printw("║  ");
    for(int i = 0; i < array_size; i++)
    {
        printw("%c", charDisplay[i]);
    }
    printw("  ║        PRESS TAB FOR EXTRA OPTIONS:\n");

    move (prev_y, prev_x);
    refresh();
}

void calculator_display(char charDisplay[]) /// the calculator we'll be moving around on
{
    printw("╔══════════════════════════════╗\n");
    printw("║                              ║        PRESS TAB FOR EXTRA OPTIONS:\n");
    printw("╠══════════════════════════════╣\n");
    printw("║                              ║        CLEAR LINE\n");
    printw("║  1           2            3  ║        RESET\n");
    printw("║                              ║\n");
    printw("║  4           5            6  ║\n");
    printw("║                              ║\n");
    printw("║  7           8            9  ║\n");
    printw("║                              ║\n");
    printw("║              0               ║\n");
    printw("║                              ║\n");
    printw("║  +      -    =    *       /  ║        PRESS 'ESC' TO EXIT\n");
    printw("╚══════════════════════════════╝\n");

}

bool valid_pos(int ch) /// checks if the cursor is on a valid character
{

    if(ch == ' ' || ch == 0x2502 || ch == 0x2550 )
    {
        return false;
    }
    else
    {
        return true;
    }
}

void actions(char charDisplay[], size_t size){ /// it handles key arrows, enter, tab, spacebar, also the whole logic of the program overall (it shouldn't, but whatever)
    int y, x;
    long long number1, number2, result;             ///a bunch of variables that are used within this function
    bool flag_number1 = false;
    bool flag_number2 = false;
    char last_operator;
    bool extra_is_on = false;

    getyx(stdscr, y, x);    /// gets the current cursor position
    move( 6 , 15 );         /// it moves the cursor to a specific location

    int ch = getch();

    while(ch != 27)
    {
        chtype current = inch();
        int c = current & A_CHARTEXT;

        if(ch == KEY_UP && !extra_is_on) /// from now on, functions will check if a specific key is pressed and if the extra flag is on
        {
            if(c == '+' || c == '-' || c == '=' || c == '*' || c == '/')
            {
                getyx(stdscr, y, x);
                move( 10 , 15 );
            }
            else
            {
                while(c != '1' && c != '2' && c != '3')
                {
                    getyx(stdscr, y, x);
                    move( y - 1 , x );

                    current = inch();       /// useful function to extract text from the terminal output
                    c = current & A_CHARTEXT; /// conversion that I have to make bc when you get the character, it also saves its attributes, and I only want the character itself

                    if(valid_pos(c))
                    {
                        break;
                    }
                }
            }

            refresh();
        }

        if(ch == KEY_DOWN && !extra_is_on)
        {
            if(c == '7' || c == '8' || c == '9')
            {
                getyx(stdscr, y, x);
                move( 10 , 15 );
            }
            else
            {
                while(c != '+' && c != '-' && c != '=' && c != '*' && c != '/')
                {
                    getyx(stdscr, y, x);
                    move( y + 1 , x );
                    current = inch();
                    c = current & A_CHARTEXT;

                    if(valid_pos(c))
                    {
                        break;
                    }
                }

            }
            refresh();
        }

        if(ch == KEY_LEFT && !extra_is_on)
        {
            while(c != '1' && c != '4' && c != '7' && c != '0' && c != '+')
            {
                getyx(stdscr, y, x);
                move( y , x - 1);
                current = inch();
                c = current & A_CHARTEXT;

                if(valid_pos(c))
                {
                    break;
                }
            }
            refresh();
        }

        if(ch == KEY_RIGHT && !extra_is_on)
        {
            while(c != '3' && c != '6' && c != '9' && c != '0' && c != '/')
            {
                getyx(stdscr, y, x);
                move( y , x + 1);
                current = inch();
                c = current & A_CHARTEXT;

                if(valid_pos(c))
                {
                    break;
                }
            }
            refresh();
        }

        if(ch == '\n' ||ch == '\r' ||ch == ' ') /// checks if the return/spacebar keys were pressed
        {

            if(c == '+' || c == '-' || c == '*' || c == '/' || c == '=') /// special keys that do arimethic calculations, equal is also included
            {
                if(c != '=' && !extra_is_on)
                {
                    last_operator = c;
                }

                if(!flag_number1)
                {
                    number1 = atoll(charDisplay); /// turns the char array into a long long datatype
                    flag_number1 = true;

                    for(int i = 0; i < array_size; i++)
                    {
                        charDisplay[i] = ' ';
                    }
                }
                else if(!flag_number2)
                {
                    number2 = atoll(charDisplay);
                    flag_number2 = true;

                    for(int i = 0; i < array_size; i++)
                    {
                        charDisplay[i] = ' ';
                    }
                }
                else
                {
                    number1 = number2;
                    number2 = atoll(charDisplay);

                    for(int i = 0; i < array_size; i++)
                    {
                        charDisplay[i] = ' ';
                    }
                }


                    if(c == '=' && flag_number1  && flag_number2)
                    {

                        switch(last_operator)
                        {
                            case '+':
                            result = number1 + number2;
                                break;

                            case '-':
                            result = number1 - number2;
                                break;

                            case '*':
                            result = number1 * number2;
                                break;

                            case '/':
                            result = number1 / number2;
                                break;
                        }

                        snprintf(charDisplay, size, "%lld", result);

                        int amount = 0;

                        for(int i = 0; charDisplay[i] != '\0' ; i++)
                        {
                            amount++;
                        }
                        /// from line 235 to 259 I straight up lost my grip on reality
                        int array_correction = array_size - amount;
                        int counter = 0;

                        for (; array_correction <= array_size; array_correction ++)
                        {

                            if(charDisplay[array_correction] == '\0')
                            {
                                charDisplay[array_correction] = ' ';
                                continue;
                            }

                            charDisplay[array_correction] = charDisplay[counter];

                            charDisplay[counter] = ' ';
                            counter++;
                        }

                        if(counter < (array_size / 2))
                        {
                            for(;counter < array_size / 2; counter++)
                            {
                                charDisplay[counter] = charDisplay[array_size - counter - 1] = ' ';
                            }
                        }

                        array_display(charDisplay);
                    }


            }
            else
            {
                if(c == 'C')
                {
                    for (int i = 0; i < array_size; i ++)
                    {
                        charDisplay[i] = ' ';
                    }
                }
                else if (c == 'R')
                {
                    for (int i = 0; i < array_size; i ++)
                    {
                        charDisplay[i] = ' ';
                    }
                    number1 = number2 = 0;
                    flag_number1 = flag_number2 = false;
                }
                else
                {
                    for (int i = 0; i < array_size - 1; i ++)
                    {
                        charDisplay[i] = charDisplay[i + 1];
                    }


                    charDisplay[25] = c;
                }

            }


            array_display(charDisplay);
        }

        if(ch == '\t') /// it's used for switching between normal and extra mode
        {
            current = inch();
            c = current & A_CHARTEXT;

            if(c != 'R' && c!='C')
            {
                move( 3 , 38 );
                addch('>');
                move( 3 , 40 );
                curs_set(0);
                extra_is_on = true;
                refresh();
            }

            if(c == 'C')
            {
                move( 3 , 38 );
                addch(' ');
                move( 4 , 38 );
                addch('>');
                move( 4 , 40 );
                refresh();
            }
            else if (c == 'R')
            {
                move( 4 , 38 );
                addch(' ');
                move( 6 , 15 );
                curs_set(1);
                extra_is_on = false;
                refresh();
            }
        }

        ch = getch();
    }

}


void program() /// function that handles the program's execution
{
    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, TRUE);
    noecho();

    char charDisplay[array_size + 1 ];

    for(int i = 0; i < array_size; i++)
    {
        charDisplay[i] = ' ';
    }

    charDisplay[26] = '\0';

    calculator_display(charDisplay);
    actions(charDisplay, sizeof(charDisplay));

    endwin();
}



int main()
{
    program(); /// maybe I shouldn't use a function for what "program()" does..

	return 0;
}
