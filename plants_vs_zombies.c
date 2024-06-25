/* By Santiago Jiménez
 * Tower defense game with Plant vs. Zombies theme
 * Solo Game*/

/* Include libraries */
#include <stdio.h> //printf, scanf, etc.
#include <windows.h> //Library for command Sleep
#include <stdlib.h> //Library for rand() and srand()
#include <time.h> //Library for time()

/* Define constants */
#define ARRAY_SIZE 8

/* Define custom data types */

/* Menu options */
typedef enum {
    play,
    minigame,
    instructions,
    levels,
    stats,
    credits,
    quit
} menu_t;

/* Level options */
typedef enum {
    lvl0,
    lvl1,
    lvl2
} level_t;

/* Node for linked list */
typedef struct node_s {
    char character;
    struct node_s *p_next;
} node_t;

/* Plant type */
typedef enum {
    peashooter,
    mortar,
} plant_type_t;

/* Structure for plant */
typedef struct {
    plant_type_t type;
    int num_plants;
} plant_t;

/* Structure for zombie */
typedef struct {
    int life;
} zombie_t;

/* Global array to store plants */
plant_t placed_plants[2];


/* Function prototypes */
void print_board(char board[5][8], int size); 
menu_t display_menu(void);
void display_instructions(void);
void display_stats(void);   
void add_node_at_front(node_t **pp_head, char value);
int remove_node_from_front(node_t **pp_head);
void create_levels_dynamic(node_t **pp_head);
void display_levels(node_t **pp_head);
void display_credits(void);
void select_level(char board[5][8], int size, int *num_zombies, int *money); 
void set_board(char board[5][8], int size);
int input_row(void);
int input_col(void);
void select_plant(plant_t *pplant);
void save_plant_types(plant_t plant);
void assign_plant(char board[5][8], int size, plant_t *pplant, int *money);
void start_game(char board[5][8], int size, int num_zombies, plant_t plant);
int walk_zombie(char board[5][8], int size, plant_t plant, zombie_t zombie[6], int count);
void peashooter_attack(char board[5][8], int size, zombie_t zombies[6], int count, int row, int col);
void mortar_attack(char board[5][8], int size, zombie_t zombies[6], int count);
int check_win_lose(int num_zombies, int zombies_reached_tower);

/* Minigame prototypes */
void start_minigame(char board[5][8], int size, plant_t plant);
int walk_zombie_minigame(char board[5][8], int size, plant_t plant, zombie_t zombie[10], int count);
void peashooter_minigame(char board[5][8], int size, zombie_t zombies[10], int count, int row, int col);
void mortar_minigame(char board[5][8], int size, zombie_t zombies[10], int count);

/* Main function */
int main(void) {

    /* Seed random number generator */
    srand(time(NULL));

    /* Declare variables */
    int money;
    int num_zombies = 0;
    menu_t menu_selection;
    level_t level;
    plant_t plant;

    /* Declare empty linked list */
    node_t *p_head = NULL;

    /* Main menu */
    printf("Welcome to Plants vs. Zombies!\n\n");

    /* Loop to keep displaying menu until user quits */
    while (menu_selection != 6) {
        /* Declare main board */
        char board[5][8] = {
                    {'#', '#', '#', '#', '#', '#', '#', '#'},
                    {'#', '#', '#', '#', '#', '#', '#', '#'},
                    {'#', '#', '#', '#', '#', '#', '#', '#'},
                    {'#', '#', '#', '#', '#', '#', '#', '#'},
                    {'#', '#', '#', '#', '#', '#', '#', '#'} };

        /* Display menu */
        menu_selection = display_menu();

        /* Clear screen */
        system("cls");

        /* Switch statement to determine menu selection */
        switch (menu_selection) 
        {
        /* Play game */
        case play:

            printf("\n");

            /* Ask user to select level and set the board */
            select_level(board, ARRAY_SIZE, &num_zombies, &money);
            Sleep(1500);
            system("cls");

            /* Print board with the selected level set */
            print_board(board, ARRAY_SIZE);

            /* Select plant type and its coordinates.
             * Keep asking while user has enough money 
             * to buy a plant. */
            do 
            {
                printf("Where do you want to place your plant?\n");

                /* Set the plant in the board */
                assign_plant(board, ARRAY_SIZE, &plant, &money);

                /* Print board with the plant set */
                print_board(board, ARRAY_SIZE);
                Sleep(500);
            } while ((money >= 50) && (plant.type != -1));

            Sleep(1000);

            /* Start the game */
            start_game(board, ARRAY_SIZE, num_zombies, plant);
            break;
        
        /* Minigame */
        case minigame:

            /* Set the money of the minigame */
            money = 350;

            /* Print board with the minigame set */
            printf("\n\n");
            print_board(board, ARRAY_SIZE);
            
            /* Select plant type and its coordinates.
             * Keep asking while user has enough money 
             * to buy a plant. */
            do
            {
                printf("Where do you want to place your plant?\n");

                /* Set the plant in the board */
                assign_plant(board, ARRAY_SIZE, &plant, &money);

                /* Print board with the plant set */
                print_board(board, ARRAY_SIZE);
            } while ((money >= 50) && (plant.type != -1));   

            Sleep(1000);

            /* Start the minigame */
            start_minigame(board, ARRAY_SIZE, plant);
            break;

        /* Display instructions */
        case instructions:

            printf("\n");
            display_instructions();
            printf("\n\n");
            Sleep(2000);
            break;
        
        /* Display levels */
        case levels:

            /* Create levels with dynamic memory and print them */
            create_levels_dynamic(&p_head);
            Sleep(1000);
            break;
        
        /* Display stats */
        case stats:

            display_stats();
            Sleep(1000);
            break;
        
        /* Display credits */
        case credits:

            display_credits();
            Sleep(1000);
            break;
        
        /* Quit game */
        case quit:

            printf("\n");
            printf("Thanks for playing!\n");
            break;
            
        default:
            break;
        }
    }   
}

/* Function to print the board */
void print_board(char board[5][8], int size) 
{
    int i;

    printf("    0 1 2 3 4 5 6 7\n");
    printf("  -------------------\n");
    printf("0 | %c %c %c %c %c %c %c %c |\n", board[0][0], board[0][1], board[0][2], board[0][3], board[0][4], board[0][5], board[0][6], board[0][7]);
    printf("1 | %c %c %c %c %c %c %c %c |\n", board[1][0], board[1][1], board[1][2], board[1][3], board[1][4], board[1][5], board[1][6], board[1][7]);
    printf("2 | %c %c %c %c %c %c %c %c |\n", board[2][0], board[2][1], board[2][2], board[2][3], board[2][4], board[2][5], board[2][6], board[2][7]);
    printf("3 | %c %c %c %c %c %c %c %c |\n", board[3][0], board[3][1], board[3][2], board[3][3], board[3][4], board[3][5], board[3][6], board[3][7]);
    printf("4 | %c %c %c %c %c %c %c %c |\n", board[4][0], board[4][1], board[4][2], board[4][3], board[4][4], board[4][5], board[4][6], board[4][7]);
    printf("  -------------------\n");

}

/* Function to display the menu and ask for a selection */
menu_t display_menu(void)
{
    /* Declare variables */
    menu_t selection;
    int sts1;
    char dummy_str[50];
    int error_flag;

    /* Display menu */
    printf("Menu:\n");
    printf("0. Play\n");
    printf("1. Minigame\n");
    printf("2. How to play?\n");
    printf("3. Levels\n");
    printf("4. Plants and Zombies Stats\n");
    printf("5. Credits\n");
    printf("6. Exit Game\n");

    /* Ask user to select a menu option */
    do
    {
        error_flag = 0;
        printf("Select an option: ");
		sts1 = scanf("%d", &selection);


		
		/* Check for invalid characters */
		if (sts1 != 1)
		{
			printf("That was not a number!\n");
			/* Flush any remaining characters */
			scanf("%s", dummy_str);
			error_flag = 1;
		}
		else
		{
			/* Check that the number is valid */
			if (selection < 0 || selection > 6)
            {
                printf("This option is not valid! \n");
                error_flag = 1;
            }
		}

    } while (error_flag == 1);

    /* Return the selection */
    return selection;
}

/* Function to display the instructions */
void display_instructions(void)
{
    /* Variables */
    FILE *fp;
    char line[100];

    /* Open the instructions file in read mode */
    fp = fopen("instructions.txt", "r");
	
    /* Check if the file was opened successfully */
	if (fp == NULL)
	{
		printf("Couldn't open the file\n");
	}

    /* Read the file line by line */
    while (fgets(line, 100, fp) != NULL)
    {
        /* Print the line */
        printf("%s", line);
    }
    /* Close the file */
    fclose(fp);
    Sleep(12000);   
}

/* Function to display the stats of the plants and zombies */
void display_stats(void)
{
    printf("\n");
    printf(">> Plants stats\n");
    printf("   >Peashooter                  >Mortar\n");
    printf("     Cost: $50                     Cost: $150\n");
    printf("     Damage: 12-20                 Damage: 25\n");
    printf("     Range: 1                      Range: All board\n");
    printf("     Cannot attack diagonally      Attacks every 3 seconds!\n");
    printf("\n");

    Sleep(6000);
    printf(">> Zombies stats\n");
    printf("    Life points: 100\n");
    printf("    Speed: 1\n");
    printf("\n");
    Sleep(4000);

}

/* Function to add nodes to a linked list */
void add_node_at_front(node_t **pp_head, char value)
{
	node_t *p_node;
	
	/* Add one node */
	p_node = malloc(sizeof(node_t));
	if (p_node == NULL)
	{
		printf("Failed to allocate memory\n");
		return;
	}
	
	/* Initialize node */
	p_node->character = value;
	
	/* The new node points to whatever p_head was pointing to (could be NULL) */
	p_node->p_next = *pp_head;
	
	/* Make the head point to the new node */
	*pp_head = p_node;
}

/* Function to delete nodes of a linked list and free the memory */
int remove_node_from_front(node_t **pp_head)
{
	node_t *p_node;
	int value;
	
	/* Is the list empty? */
	if (*pp_head == NULL)
	{
		return -1;
	}
	else
	{
		p_node = *pp_head;
		
		/* Skip node in linked list */
		*pp_head = p_node->p_next;
		
		/* Copy value */
		value = p_node->character;
		
		/* Free node */
		free(p_node);
		return 0;
	}
}

/* Function that creates the levels using a linked list and displays it */
void create_levels_dynamic(node_t **pp_head)
{
    int value = 0;

    /* Create level 0 */
    printf("Levels:\n");
    printf("Level 0:\n");

    /* Add nodes of level 0 to the linked list */
    for (int i = 0; i < 40; i++) {
        /* Set the path of the zombie */
        if (i == 22 || i == 23 || i == 14 || i == 13 || i == 12 || i == 11 || i == 10 || i == 9 || i == 8) {
            add_node_at_front(pp_head, ' ');
        } else {
            add_node_at_front(pp_head, '#');
        }
    }

    /* Display the linked list */
    display_levels(pp_head);
    Sleep(2000);
    printf("\n");

    /* Free the memory of the linked list */
    while (value == 0)
    {
        value = remove_node_from_front(pp_head);
    }

    /* Create level 1 */
    printf("Level 1:\n");

    /* Add nodes of level 1 to the linked list */
    for (int i = 0; i < 40; i++) {
        /* Set the path of the zombie */
        if (i == 14 || i == 15 || i == 22 || i == 30 || i == 29 || i == 28 || i == 27 || i == 8 || i == 9 || i == 10 || i == 11 || i == 19) {
            add_node_at_front(pp_head, ' ');
        } else {
            add_node_at_front(pp_head, '#');
        }
    }

    /* Display the linked list */
    display_levels(pp_head);
    Sleep(2000);
    printf("\n");

    /* Free the memory of the linked list */
    value = 0;
    while (value == 0)
    {
        value = remove_node_from_front(pp_head);
    }

    /* Create level 2 */
    printf("Level 2:\n");

    /* Add nodes of level 2 to the linked list */
    for (int i = 0; i < 40; i++) {
        /* Set the path of the zombie */
        if (i == 22 || i == 23 || i == 12 || i == 13 || i == 14 || i == 4 || i == 3 || i == 2 || i == 8 || i == 9 || i == 10) {
            add_node_at_front(pp_head, ' ');
        } else {
            add_node_at_front(pp_head, '#');
        }
    }

    /* Display the linked list */
    display_levels(pp_head);
    Sleep(2000);
    printf("\n");

    /* Free the memory of the linked list */
    value = 0;
    while (value == 0)
    {
        value = remove_node_from_front(pp_head);
    }
}

/* Function to display the levels (linked list) */
void display_levels(node_t **pp_head) {
    node_t *p_node;
	int count_col = 0;

    /* Grab the first node */
	p_node = *pp_head;

    /* Loop through the linked list */
	while (p_node != NULL)
	{
        /* If it is printing column 7, print a next line */
        if (count_col == 7) {
            printf("%c\n", p_node->character);
		    p_node = p_node->p_next;
            count_col = 0;
        } else {
            printf("%c ", p_node->character);
            p_node = p_node->p_next;
            count_col++;
        }		
	} 
}

/* Function to display the credits */
void display_credits(void)
{
    printf("        ---------\n");
    printf("        |Credits| \n");
    printf("        ---------\n");

    printf("Created by:\n");
    printf("    > Santiago Jimenez\n");
    printf("\n");

    printf("Contributors:\n");
    printf("    > Juan Carlos Rojas\n");
    printf("\n");

    Sleep(2000);
}

/* Function that asks the user to choose a level and sets the board and set variables depending of the selected level */
void select_level(char board[5][8], int size, int *pnum_zombies, int *pmoney) 
{
    /* Variables */
    level_t level;
    int sts1;
    char dummy_str[50];
    int error_flag;

    /* Print the levels options */
    printf("Select level: \n");
    printf("0 - Easy\n");
    printf("1 - Medium\n");
    printf("2 - Hard\n");
    printf("\n");

    /* Get the level from the user */
    do
    {
        error_flag = 0;
        printf("Select an option: ");
		sts1 = scanf("%d", &level);


		
		/* Check for invalid characters */
		if (sts1 != 1)
		{
			printf("That was not a number!\n");
			/* Flush any remaining characters */
			scanf("%s", dummy_str);
			error_flag = 1;
		}
		else
		{
			/* Check that the number is valid */
			if (level < 0 || level > 2)
            {
                printf("This option is not valid! \n");
                error_flag = 1;
            }
		}

    } while (error_flag == 1);

    /* Set the board and set variables depending of the selected level */
    switch (level) {
    case lvl0:
        printf("Level 0 selected\n");
        board[2][0] = ' ';
        board[2][1] = ' ';
        board[3][1] = ' ';
        board[3][2] = ' ';
        board[3][3] = ' ';
        board[3][4] = ' ';
        board[3][5] = ' ';
        board[3][6] = ' ';
        board[3][7] = ' ';

        /* Set the number of zombies */
        *pnum_zombies = 2;
        /* Set the money */
        *pmoney = 250;
        break;
    case lvl1:
        printf("Level 1 selected\n");
        board[3][0] = ' ';
        board[3][1] = ' ';
        board[2][1] = ' ';
        board[1][1] = ' ';
        board[1][2] = ' ';
        board[1][3] = ' ';
        board[1][4] = ' ';
        board[2][4] = ' ';
        board[3][4] = ' ';
        board[3][5] = ' ';
        board[3][6] = ' ';
        board[3][7] = ' ';

        /* Set the number of zombies */
        *pnum_zombies = 4;
        /* Set the money */
        *pmoney = 150;
        break;
    case lvl2:
        printf("Level 2 selected\n");
        board[2][0] = ' ';
        board[2][1] = ' ';
        board[3][1] = ' ';
        board[3][2] = ' ';
        board[3][3] = ' ';
        board[4][3] = ' ';
        board[4][4] = ' ';
        board[4][5] = ' ';
        board[3][5] = ' ';
        board[3][6] = ' ';
        board[3][7] = ' ';

        /* Set the number of zombies */
        *pnum_zombies = 6;
        /* Set the money */
        *pmoney = 150;
        break;
    }
}

/* Function to input row */
int input_row(void) {
    int row;
    int sts1;
    char dummy_str[50];
    int error_flag;

    do
    {
        error_flag = 0;
        printf("Enter row: ");
		sts1 = scanf("%d", &row);


		
		/* Check for invalid characters */
		if (sts1 != 1)
		{
			printf("That was not a number!\n");
			/* Flush any remaining characters */
			scanf("%s", dummy_str);
			error_flag = 1;
		}
		else
		{
			/* Check that the number is positive */
			if (row < 0 || row > 4)
            {
                printf("This row is not valid! \n");
                error_flag = 1;
            }
		}

    } while (error_flag == 1);

    return row;

}

/* Function to input column */
int input_col(void) {
    int col;
    int sts1;
    char dummy_str[50];

    int error_flag;

    do
    {
        error_flag = 0;
        printf("Enter column: ");
		sts1 = scanf("%d", &col);


		
		/* Check for invalid characters */
		if (sts1 != 1)
		{
			printf("That was not a number!\n");
			/* Flush any remaining characters */
			scanf("%s", dummy_str);
			error_flag = 1;
		}
		else
		{
			/* Check that the number is positive */
			if (col < 0 || col > 7)
            {
                printf("This column is not valid! \n");
                error_flag = 1;
            }
		}

    } while (error_flag == 1);

    return col;
}

/* Function to input plant type */
void select_plant(plant_t *pplant) {
    int sts1;
    char dummy_str[50];
    int error_flag;

    do
    {
        error_flag = 0;

        /* Print the plant options */
        printf("Select plant to place: \n");
        printf("0. Peashooter -$50\n");
        printf("1. Mortar     -$150\n");
        sts1 = scanf("%d", pplant);

        /* Check for invalid characters */
        if (sts1 != 1)
        {
            printf("That was not a number!\n");
            /* Flush any remaining characters */
            scanf("%s", dummy_str);
            error_flag = 1;
        }
        else
        {
            /* Check that the number is positive */
            if (pplant->type == -1)
                break;
            if (pplant->type < 0 || pplant->type > 1)
            {
                printf("This plant is not valid! \n");
                error_flag = 1;
            }
        }

    } while (error_flag == 1);

    /* Save the plant type */
    save_plant_types(*pplant);
}

/* Function to save plant types */
void save_plant_types(plant_t plant) {
    switch (plant.type) {
    case peashooter:
        placed_plants[0].type = peashooter;
        break;
    case mortar:
        placed_plants[1].type = mortar;
        break;
    default:
        break;
    }
}

/* Function that asks the user for plant type, and the coordinates of the plant. Also sets the plant in the board. */
void assign_plant(char board[5][8], int size, plant_t *pplant, int *pmoney)
{
    /* Variables */
    int row;
    int col;
    int error_flag = 0;

    /* Set plants */
    do {
        printf("You have $%d money left\n", *pmoney);

        /* Ask the user for the plant type */
        select_plant(pplant);
        if (pplant->type == -1)
            return;

        /* Ask the user for the coordinates */
        row = input_row();
        col = input_col();
        printf("\n");

        /* Place the plant in the board */
        switch (pplant->type)
        {
        case peashooter:
            
            /* Check if the user has enough money */
            if (*pmoney < 50)
            {
                printf("You do not have enough money to place this plant!\n");
                error_flag = 1;
                break;
            }

            /* Check if the plant can be placed */
            if (board[row][col] == '#')
            {
                /* Place the plant */
                board[row][col] = 'X';

                /* Subtract the money */
                *pmoney = *pmoney - 50;

                /* Add the plant to the global array */
                placed_plants[0].num_plants = placed_plants[0].num_plants + 1;
            } else {
                printf("This space is already occupied or invalid!\n");
                error_flag = 1;
            }
            break;
        case mortar:

            /* Check if the user has enough money */
            if (*pmoney < 150)
            {
                printf("You do not have enough money to place this plant!\n");
                error_flag = 1;
                break;
            }

            /* Check if the plant can be placed */
            if (board[row][col] == '#')
            {
                /* Place the plant */
                board[row][col] = 'H';

                /* Subtract the money */
                *pmoney = *pmoney - 150;

                /* Add the plant to the global array */
                placed_plants[1].num_plants = placed_plants[1].num_plants + 1;
            } else {
                printf("This space is already occupied or invalid!\n");
                error_flag = 1;
            }
            break;
        default:
            break;
        }
        
    } while ((error_flag == 1) && (pplant->type != -1));
    printf("\n");
}

/* Function that sets the zombies and starts the game */
void start_game(char board[5][8], int size, int num_zombies, plant_t plant)
{
    /* Variables */
    zombie_t zombies[6];
    int zombie_count = 0; // Counts the number of zombies deployed
    int zombies_reached_tower = 0; //number of zombies that reached the tower
    int check_win = 1;
    
    /* Set zombies life */
    switch (num_zombies)
    {
    case 2:
        zombies[0].life = 100;
        zombies[1].life = 100;
        break;
    case 4:
        zombies[0].life = 100;
        zombies[1].life = 100;
        zombies[2].life = 100;
        zombies[3].life = 100;
        break;
    case 6:
        zombies[0].life = 100;
        zombies[1].life = 100;
        zombies[2].life = 100;
        zombies[3].life = 100;
        zombies[4].life = 100;
        zombies[5].life = 100;
        break;
    
    default:
        break;
    }

    /* Start zombies' horde */
    while (zombie_count < num_zombies)
    {
        /* Deploy zombie on the board */
        zombies_reached_tower += walk_zombie(board, size, plant, zombies, zombie_count);

        /* reset zombie path */    
        set_board(board, ARRAY_SIZE);

        /* Check if the user has lost */
        check_win = check_win_lose(num_zombies, zombies_reached_tower);
        if (check_win == 0)
        {
            printf("You lost! %d zombie(s) reached the tower.\n\n", zombies_reached_tower);
            Sleep(500);
            placed_plants[0].type = -1;
            placed_plants[0].num_plants = 0;

            placed_plants[1].type = -1;
            placed_plants[1].num_plants = 0;
            break;
        }

        Sleep(1000);
        zombie_count++;
    }

    /* Check if the user has won */
    if (check_win == 1)
    {
        printf("\nYou won! %d zombie(s) reached the tower.\n\n", zombies_reached_tower);
        Sleep(1500);
        placed_plants[0].type = -1;
        placed_plants[0].num_plants = 0;

        placed_plants[1].type = -1;
        placed_plants[1].num_plants = 0;
    }

    Sleep(5000);
}

/* Function to deploy zombies and display their way on the board */
int walk_zombie(char board[5][8], int size, plant_t plant, zombie_t zombie[], int count)
{
    /* Variables */
    int walked_spaces = 0; //spaces walked by zombie
    int zombies_reached_tower = 0;

    /* Set starting position */
    int row = 2;
    int col = 0;

    /* Loop for the zombie to follow the path */
    while (zombie[count].life > 0)
    {
        /* If path is clear, move the zombie there */
        if (board[row][col] == ' ')
        {
            board[row][col] = 254;

        } else if (board[row+1][col] == ' ')
        {
            board[row+1][col] = 254;
            row++;
        } else if (board[row-1][col] == ' ')
        {
            board[row-1][col] = 254;
            row--;
        } else if (board[row][col+1] == ' ')
        {
            board[row][col+1] = 254;
            col++;
        } else if (board[row][col-1] == ' ')
        {
            board[row][col-1] = 254;
            col--;
        } else if (board[row+1][col+1] == ' ')
        {
            board[row+1][col+1] = 254;
            row++;
            col++;
        } else if (board[row+1][col-1] == ' ')
        {
            board[row+1][col-1] = 254;
            row++;
            col--;
        } else if (board[row-1][col+1] == ' ')
        {
            board[row-1][col+1] = 254;
            row--;
            col++;
        } else if (board[row-1][col-1] == ' ')
        {
            board[row-1][col-1] = 254;
            row--;
            col--;
        } else {
            printf("Zombie %d has reached the tower!\n\n", count+1);
            zombies_reached_tower += 1;
            break;
        }
        
        /* Print the board every time the zombie moves one space */
        Sleep(700);
        print_board(board, ARRAY_SIZE);

        /* Check if the user put a mortar plant */
        if ((walked_spaces == 3) && (zombie[count].life > 0) && (placed_plants[1].type == mortar) && (placed_plants[1].num_plants > 0))
        {
            /* Hit the zombie with mortar(s) depending on how many mortar are on the board */
            for (int j = 0; j < placed_plants[1].num_plants; j++)
            {
                /* Hit the zombie */
                mortar_attack(board, ARRAY_SIZE, zombie, count);        
                Sleep(500);
            }

            /* Print if the zombie was hit by the mortar(s) */
            if (placed_plants[1].num_plants > 1){
                printf("Mortars double hit the Zombie!\n");
                Sleep(500);
            } else {
                printf("Mortar hit the Zombie!\n");
                Sleep(500);
            }

            /* Reset the counter of spaces walked by the zombie */
            walked_spaces = 0;
        }

        /* Increase the counter of spaces walked by the zombie */
        walked_spaces++;

        /* Checks if the user put a peashooter plant and hits the zombie */
        peashooter_attack(board, ARRAY_SIZE, zombie, count, row, col);

        /* Clear last step of the zombie  */
        board[row][col] = -1;  
        
    }

    /* Check if the zombie is dead */
    if (zombie[count].life <= 0)
    {
        printf("Zombie %d has been killed!\n", count+1);
        Sleep(500);
    }

    /* Return the number of zombies that reached the tower */
    return zombies_reached_tower;
}

/* Function to hit a zombie with a mortar */
void mortar_attack(char board[5][8], int size, zombie_t zombies[6], int count)
{
    zombies[count].life -= 25;
    //printf("Zombie %d has %d life left.\n", count+1, zombies[count].life);
}

/* Function to hit a zombie with a peashooter plant */
void peashooter_attack(char board[5][8], int size, zombie_t zombies[6], int count, int row, int col)
{
    /* Checks if the is a peashooter nearby */
    if (board[row+1][col] == 'X')
    {
        /* Hit the zombie */
        zombies[count].life -= rand() % 20 + 12;
        //printf("Zombie %d has %d life left\n", count+1, zombies[count].life);

    } else if (board[row][col+1] == 'X')
    {
        /* Hit the zombie */
        zombies[count].life -= rand() % 20 + 12;
        //printf("Zombie %d has %d life left\n", count+1, zombies[count].life);

    } else if (board[row-1][col] == 'X')
    {
        /* Hit the zombie */
        zombies[count].life -= rand() % 20 + 12;
        //printf("Zombie %d has %d life left\n", count+1, zombies[count].life);

    } else if (board[row][col-1] == 'X')
    {
        /* Hit the zombie */
        zombies[count].life -= rand() % 20 + 12;
        //printf("Zombie %d has %d life left\n", count+1, zombies[count].life);
    }
}

/* Function to set the zombie path to the initial state */
void set_board(char board[5][8], int size)
{
    /* Set starting position */
    int row = 2;
    int col = 0;

    /* Loop that erases the zombie trail and clear it */
    while (1) {
        if (board[row][col] == -1)
        {
            board[row][col] = ' ';

        } else if (board[row+1][col] == -1)
        {
            board[row+1][col] = ' ';
            row++;
        } else if (board[row-1][col] == -1)
        {
            board[row-1][col] = ' ';
            row--;
        } else if (board[row][col+1] == -1)
        {
            board[row][col+1] = ' ';
            col++;
        } else if (board[row][col-1] == -1)
        {
            board[row][col-1] = ' ';
            col--;
        } else if (board[row+1][col+1] == -1)
        {
            board[row+1][col+1] = ' ';
            row++;
            col++;
        } else if (board[row+1][col-1] == -1)
        {
            board[row+1][col-1] = ' ';
            row++;
            col--;
        } else if (board[row-1][col+1] == -1)
        {
            board[row-1][col+1] = ' ';
            row--;
            col++;
        } else if (board[row-1][col-1] == -1)
        {
            board[row-1][col-1] = ' ';
            row--;
            col--;
        } else {
            break;
        }
    }
}

/* Function to check if the user has won or lost */
int check_win_lose(int num_zombies, int zombies_reached_tower)
{
    /* Switch to check if the user has won or lost depending on the total number of zombies */
    switch (num_zombies)
    {
    case 2:
        /* If one zombie reached the tower, the user has lost */
        if (zombies_reached_tower >= 1)
        {
            return 0;
        } else {
            return 1;
        }
        break;
    case 4:
    case 6:
    case 10:
        /* If two zombies reached the tower, the user has lost */
        if (zombies_reached_tower >= 2)
        {
            return 0;
        } else {
            return 1;
        }
        break;    

    default:
        break;
    }
}

/* Function to start the mini-game */
void start_minigame(char board[5][8], int size, plant_t plant)
{
    /* Variables */
    zombie_t zombies[10]; // Array of 10 zombies
    int deployed_zombies_count = 0;
    int zombies_reached_house = 0;
    int check = 1;

    /* Set the zombies' life */
    for (int i = 0; i < 10; i++)
    {
        zombies[i].life = 100;
    }
    
    /* Start the zombies' horde */
    while (deployed_zombies_count < 10)
    {
        /* Deploy zombie on the board */
        zombies_reached_house += walk_zombie_minigame(board, size, plant, zombies, deployed_zombies_count);

        /* Check if the user has won or lost */
        check = check_win_lose(10, zombies_reached_house);

        /* Check if the user has lost */
        if (check == 0)
        {
            printf("You lost! %d zombie(s) reached the house.\n\n", zombies_reached_house);
            Sleep(500);
            placed_plants[0].type = -1;
            placed_plants[0].num_plants = 0;

            placed_plants[1].type = -1;
            placed_plants[1].num_plants = 0;

            break;
        }

        deployed_zombies_count++;
        Sleep(1000);
    }

    /* Check if the user has won */
    if (check == 1)
    {
        printf("\nYou won! %d zombie(s) reached the house.\n\n", zombies_reached_house);
        Sleep(1500);
        placed_plants[0].type = -1;
        placed_plants[0].num_plants = 0;

        placed_plants[1].type = -1;
        placed_plants[1].num_plants = 0;
    }
}

/* Function to deploy the zombies on the board */
int walk_zombie_minigame(char board[5][8], int size, plant_t plant, zombie_t zombie[10], int count)
{
    /* Variables */
    int spaces_walked = 0;
    int zombies_reached_house = 0;
    int count_plants = 0;

    /* Set the zombie's initial position */
    int row = rand() % 4; // Start from a random row
    int col = 7;

    /* Move the zombie one column to the left until it reaches the house */
    while (zombie[count].life > 0)
    {
        if (col >= 0)
        {
            board[row][col] = 254;
            col--;

        } else {
            zombies_reached_house++;
            break;
        }

        /* Print the board after every step of the zombie */
        Sleep(700);
        print_board(board, ARRAY_SIZE);

        /* Hit the zombie with a mortar every two steps */
        if ((spaces_walked == 2) && (zombie[count].life > 0) && (placed_plants[1].type == mortar) && (placed_plants[1].num_plants > 0))
        {
            /* Hit the zombie 'n' number of times depending on how many mortar are on the board */
            for (int j = 0; j < placed_plants[1].num_plants; j++){
                mortar_minigame(board, size, zombie, count);  
                Sleep(500);
            }

            /* Reset the counter of spaces walked by the zombie */
            spaces_walked = 0;
        }
        /* Increase the counter of spaces walked by the zombie */
        spaces_walked++;

        /* Count peashooters on the board */
        for (int i = 0; i < size; i++)
        {
            if (board[row][i] == 'X')
            {
                count_plants++;
            }
        }

        /* Hit the zombie with peashooter */
        for (int i = 0; i < count_plants; i++)
        {
            peashooter_minigame(board, ARRAY_SIZE, zombie, count, row, col);
        }

        /* Reset the counter of plants on the board */
        count_plants = 0;

        /* Clear last step of the zombie */
        board[row][col+1] = '#';
    }

    /* Return the number of zombies that reached the house */
    return zombies_reached_house;  
}

/* Function to hit the zombie with a peashooter */
void peashooter_minigame(char board[5][8], int size, zombie_t zombies[10], int count, int row, int col)
{

    /* If zombie is alive and there is a peashooter on the board */
    if (zombies[count].life > 0)
    {
        /* If there is a peashooter on the same row as the zombie, hit the zombie */
        if ((board[row][0] == 'X') || (board[row][1] == 'X') || (board[row][2] == 'X') || (board[row][3] == 'X')
        || (board[row][4] == 'X') || (board[row][5] == 'X') || (board[row][6] == 'X') || (board[row][7] == 'X'))
        {
            /* Hit the zombie */
            zombies[count].life -= 10;
            //printf("Zombie %d has %d life left\n", count+1, zombies[count].life);

            if (zombies[count].life <= 0)
            {
                printf("Zombie %d has been killed by a peashooter!\n", count+1);
            }

        }
    }
    
}

/* Function to hit the zombie with a mortar */
void mortar_minigame(char board[5][8], int size, zombie_t zombies[10], int count)
{
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 8; j++){
            
            /* If there is a mortar on the board, hit the zombie */
            if (board[i][j] == 'H'){

                /* Hit the zombie */
                zombies[count].life -= rand() % 25 + 20;
                //printf("Zombie %d has %d life left\n", count+1, zombies[count].life);
            }
        }
    }

    if (zombies[count].life <= 0)
    {
        printf("Zombie %d has been killed by a mortar!\n", count+1);
    }
}