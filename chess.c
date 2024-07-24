#include <stdio.h>
#include <string.h>
#include <stdlib.h>


enum Type {
    NONE = 0,
    PAWN = 1,
    HORSE = 2,
    CASTLE = 3,
    BISHOP = 4,
    QUEEN = 5,
    KING = 6,
};

enum Color {
    WHITE = 0,
    BLACK = 1,
    NOCOLOR = 2,
};

struct Piece {
    enum Type type;
    enum Color color;
};


struct Piece board[8][8];

// Convert pieces to string
void piece_to_string(struct Piece piece, char *str){
    char *type_str;
    char *color_str;

    switch(piece.type){
        case NONE: type_str = "Empty"; break;
        case PAWN: type_str = "Pawn"; break;
        case HORSE: type_str = "Horse"; break;
        case BISHOP: type_str = "Bishop"; break;
        case CASTLE: type_str = "Castle"; break;
        case QUEEN: type_str = "Queen"; break;
        case KING: type_str = "King"; break;
        default: type_str = "Unassigned Type"; break;
    }

    switch(piece.color){
        case WHITE: color_str = "White"; break;
        case BLACK: color_str = "Black"; break;
        case NOCOLOR: color_str = "Empty"; break;
        default: color_str = "Unassigned Type"; break;
    }

    sprintf(str, "%s-%s", type_str, color_str);
}



//Sets up the board
void set_up_board() {
    
    // White Colors
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 8 ; j++){
            board[i][j].color = WHITE;
        }
    }

    // Black Colors
    for(int i = 6; i < 8; i++){
        for(int j = 0; j < 8 ; j++){
            board[i][j].color = BLACK;
        }
    }

    // White types
    board[0][0].type = CASTLE;
    board[0][1].type = HORSE;
    board[0][2].type = BISHOP;
    board[0][3].type = QUEEN;
    board[0][4].type = KING;
    board[0][5].type = BISHOP;
    board[0][6].type = HORSE;
    board[0][7].type = CASTLE;
    
    //Pawns
    for(int i = 0; i < 8; i++){
        board[1][i].type = PAWN;
        board[6][i].type = PAWN;
    }

    //Black Pieces
    board[7][0].type = CASTLE;
    board[7][1].type = HORSE;
    board[7][2].type = BISHOP;
    board[7][4].type = QUEEN;
    board[7][3].type = KING;
    board[7][5].type = BISHOP;
    board[7][6].type = HORSE;
    board[7][7].type = CASTLE;
   

    //Empty pieces
    for(int i = 2; i < 6; i++){
        for(int j = 0; j < 8; j++){
            board[i][j].type = NONE;
            board[i][j].color = NOCOLOR;
        }
    }
}



// Prints the chess board
void print_board() {

    char temp[25];
    for(int i = 7; i > -1; i--){
        for(int j = 0; j < 8; j++){
            piece_to_string(board[i][j], temp);
            if(board[i][j].type == NONE){
                printf("| XXXXXXXXXX | ");
            } else {
                printf("| %s | ", temp);
            }
        }
        printf("\n\n");
    }
    
}

void print_divider(){

    printf("\n\n\n");

    for(int i = 0; i < (2 << 6) ; i++){
        printf("-");
    }

    printf("\n\n\n");

}



int* chess_notation_decoder(char *str) {

    int* res = (int*)malloc(2 * sizeof(int));
    res[0] = -1;
    res[1] = -1;

    if(strlen(str) != 2){
        printf("Not valid Notation\n");
        return res;
    }

    switch(str[0]){
        case 'A': res[1] = 0; break;
        case 'B': res[1] = 1; break;
        case 'C': res[1] = 2; break;
        case 'D': res[1] = 3; break;
        case 'E': res[1] = 4; break;
        case 'F': res[1] = 5; break;
        case 'G': res[1] = 6; break;
        case 'H': res[1] = 7; break;
    };

    switch(str[1]){
        case '1': res[0] = 0; break;
        case '2': res[0] = 1; break;
        case '3': res[0] = 2; break;
        case '4': res[0] = 3; break;
        case '5': res[0] = 4; break;
        case '6': res[0] = 5; break;
        case '7': res[0] = 6; break;
        case '8': res[0] = 7; break;
    };

    if(res[0] == -1 || res[1] == -1){
        printf("Not valid Notation\n");
    }

    return res;

}


void force_move(char *from, char *to){

    int* from_pos = chess_notation_decoder(from);
    int* to_pos = chess_notation_decoder(to);

    //printf("%d %d\n", from_pos[0], from_pos[1]);
    //printf("%d %d\n", to_pos[0], to_pos[1]);


    board[to_pos[0]][to_pos[1]].type =  board[from_pos[0]][from_pos[1]].type;
    board[to_pos[0]][to_pos[1]].color =  board[from_pos[0]][from_pos[1]].color;
    
    board[from_pos[0]][from_pos[1]].type = NONE;
    board[from_pos[0]][from_pos[1]].color = NOCOLOR;
    
}



int pawn_logic(int *from_pos, int *to_pos){
    
    //White pawn logic
    if(board[from_pos[0]][from_pos[1]].color == WHITE){

        //Pawn steps one further
        if( (from_pos[0] + 1 == to_pos[0]) && (from_pos[1] == to_pos[1]) ){
            if(board[to_pos[0]][to_pos[1]].type == NONE){
                return 1;
            }
        }
        
        //Pawn takes a piece to the right
        
        if( (from_pos[0] + 1 == to_pos[0]) && (from_pos[1] + 1 == to_pos[1]) ){
            if(board[to_pos[0]][to_pos[1]].type != NONE){
                return 1;
            }
        }

        //Pawn takes a piece to the left
        if( (from_pos[0] + 1 == to_pos[0]) && (from_pos[1] - 1 == to_pos[1]) ){
            if(board[to_pos[0]][to_pos[1]].type != NONE){
                return 1;
            }
        }

        //Logic of first move of pawn
        if( (from_pos[0] + 2 == to_pos[0]) && (from_pos[1] == to_pos[1]) ){
           //Checks if pawn is on first roaw
           if(from_pos[0] == 1){
                if(board[to_pos[0]][to_pos[1]].type == NONE){
                    return 1;
            }
        }
         
        //Implement En passant here
        }
    }


    if(board[from_pos[0]][from_pos[1]].color == BLACK){

        //Pawn steps one further
        if( (from_pos[0] - 1 == to_pos[0]) && (from_pos[1] == to_pos[1]) ){
            if(board[to_pos[0]][to_pos[1]].type == NONE){
                return 1;
            }
        }
        
        //Pawn takes a piece to the right
        if( (from_pos[0] - 1 == to_pos[0]) && (from_pos[1] + 1 == to_pos[1]) ){
            if(board[to_pos[0]][to_pos[1]].type != NONE){
                return 1;
            }
        }

        //Pawn takes a piece to the left
        if( (from_pos[0] - 1 == to_pos[0]) && (from_pos[1] - 1 == to_pos[1]) ){
            if(board[to_pos[0]][to_pos[1]].type != NONE){
                return 1;
            }
        }

        //Logic of first move of pawn
        if( (from_pos[0] - 2 == to_pos[0]) && (from_pos[1] == to_pos[1]) ){
           //Checks if pawn is on first roaw
           if(from_pos[0] == 6){
                if(board[to_pos[0]][to_pos[1]].type == NONE){
                    return 1;
            }
        }
         
        //Implement En passant here
        }
    }


    return 0;
    
}


int rook_logic(int *from_pos, int *to_pos){
    
    if(from_pos[0] == to_pos[0]){
        
        int from = from_pos[1];
        int to = to_pos[1];

        if(from < to){
            for(int i = from+1; i < 8; i++){
                if(i == to){
                    return 1;
                }

                if(board[from_pos[0]][i].type != NONE){
                    return 0;
                }
            }
        }

        if(from > to){
            for(int i = from-1; i > -1; i--){
                if(i == to){
                    return 1;
                }

                if(board[from_pos[0]][i].type != NONE){
                    return 0;
                }
            }
        }
    }

    if(from_pos[1] == to_pos[1]){
       

       int from = from_pos[0];
       int to = to_pos[0];
       
        if(from < to){
            for(int i = from+1; i < 8; i++){
                if(i == to){
                    return 1;
                }

                if(board[i][from_pos[1]].type != NONE){
                   return 0;
                }
            }
        }

        if(from > to){
            for(int i = from-1; i > -1; i--){
                if(i == to){
                    return 1;
                }

                if(board[i][from_pos[1]].type != NONE){
                    return 0;
                }
            }
        }
    }

    return 0;

}   




int horse_logic(int *from_pos, int *to_pos){

    // Moving 2 vertically
    if(from_pos[0] + 2 == to_pos[0] && from_pos[1] + 1 == to_pos[1]){
        return 1;        
    }

    if(from_pos[0] + 2 == to_pos[0] && from_pos[1] - 1 == to_pos[1]){
        return 1;        
    }

    if(from_pos[0] - 2 == to_pos[0] && from_pos[1] + 1 == to_pos[1]){
        return 1;        
    }

    if(from_pos[0] - 2 == to_pos[0] && from_pos[1] - 1 == to_pos[1]){
        return 1;        
    }


    // Moving 2 horizonatally
    if(from_pos[0] + 1 == to_pos[0] && from_pos[1] + 2 == to_pos[1]){
        return 1;        
    }

    if(from_pos[0] + 1 == to_pos[0] && from_pos[1] - 2 == to_pos[1]){
        return 1;        
    }

    if(from_pos[0] - 1 == to_pos[0] && from_pos[1] + 2 == to_pos[1]){
        return 1;        
    }

    if(from_pos[0] - 1 == to_pos[0] && from_pos[1] - 2 == to_pos[1]){
        return 1;        
    }


    return 0;

}



int bishop_logic(int *from_pos, int *to_pos){


    // Seperate the 4 directions the bishop can go 

    if(from_pos[0] < to_pos[0] && from_pos[1] < to_pos[1]){

    
        int move_direction = (from_pos[0] > from_pos[1]) ? from_pos[0] : from_pos[1];

                    

        for(int i  = 1; i < 8 - move_direction; i++){

            int direction_x = from_pos[0] + i;
            int direction_y = from_pos[1] + i;
            
            

            if( (direction_x == to_pos[0]) && (direction_y == to_pos[1]) ){
                return 1;
            }

            if(board[direction_x][direction_y].type != NONE){
                return 0;
            }

        
        }
    }


    if(from_pos[0] < to_pos[0] && from_pos[1] > to_pos[1]){

        int move_direction = (8 - from_pos[0] < from_pos[1]) ? (8 -from_pos[0]) : from_pos[1];

        for(int i  = 1; i < move_direction+1; i++){

            int direction_x = from_pos[0] + i;
            int direction_y = from_pos[1] - i;
            
            

            if( (direction_x == to_pos[0]) && (direction_y == to_pos[1]) ){
                return 1;
            }

            if(board[direction_x][direction_y].type != NONE){
                return 0;
            }
        
        }


    }
    


    if(from_pos[0] > to_pos[0] && from_pos[1] < to_pos[1]){

        int move_direction = (from_pos[0] > 8 - from_pos[1]) ? (8 -from_pos[0]) : from_pos[1];

        for(int i  = 1; i < move_direction+1; i++){

            int direction_x = from_pos[0] + i;
            int direction_y = from_pos[1] - i;
            
            

            if( (direction_x == to_pos[0]) && (direction_y == to_pos[1]) ){
                return 1;
            }

            if(board[direction_x][direction_y].type != NONE){
                return 0;
            }
        
        }


    }
    

    if(from_pos[0] > to_pos[0] && from_pos[1] > to_pos[1]){
        
        int move_direction = (from_pos[0] < from_pos[1]) ? from_pos[0] : from_pos[1];
            

        for(int i  = 1; i < move_direction+1; i++){

            int direction_x = from_pos[0] - i;
            int direction_y = from_pos[1] - i;
                    

            if( (direction_x == to_pos[0]) && (direction_y == to_pos[1]) ){
                return 1;
            }

            if(board[direction_x][direction_y].type != NONE){
                return 0;
            }

        
        }


    }
    

    return 0;
}




int is_move_legal(int *from_pos, int *to_pos){


    if(board[from_pos[0]][from_pos[1]].color == board[to_pos[0]][to_pos[1]].color){
        return 0;
    }


    int piece = board[from_pos[0]][from_pos[1]].type;




    if(piece == PAWN){
        if(pawn_logic(from_pos, to_pos)){
            return 1;
        }
    }

    if(piece == CASTLE){
        if(rook_logic(from_pos, to_pos)){
            return 1;
        }
    }

    if(piece == HORSE){
        if(horse_logic(from_pos, to_pos)){
            return 1;
        }
    }

    if(piece == BISHOP){
        if(bishop_logic(from_pos, to_pos)){
            return 1;
        }
    }


    return 0;
}



void move(char *from, char *to){

    int* from_pos = chess_notation_decoder(from);
    int* to_pos = chess_notation_decoder(to);
    
    if(is_move_legal(from_pos, to_pos)){
        
        board[to_pos[0]][to_pos[1]].type =  board[from_pos[0]][from_pos[1]].type;
        board[to_pos[0]][to_pos[1]].color =  board[from_pos[0]][from_pos[1]].color;
    
        board[from_pos[0]][from_pos[1]].type = NONE;
        board[from_pos[0]][from_pos[1]].color = NOCOLOR;

    } else {
        printf("Not legal\n");
    }


}



void game_loop(){

    set_up_board();
    print_board();

    char *from_str = (char*)malloc(2 * sizeof(char));
    char *to_str = (char*)malloc(2 * sizeof(char)); 

    while(1){
        print_divider();

        printf("Which piece? ");

        scanf("%s", from_str);

        getchar();

        printf("To where? ");

        scanf("%s", to_str);

        getchar();

        move(from_str, to_str);

        print_divider();

        print_board();

    }
}





int main() {   

    game_loop();


};