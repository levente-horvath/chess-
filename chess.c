#include <stdio.h>
#include <string.h>


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
enum Color current_turn = WHITE;


// Tracking picies for castling
int white_king_moved = 0;
int black_king_moved = 0;
int white_rook_a_moved = 0;
int white_rook_h_moved = 0;
int black_rook_a_moved = 0;
int black_rook_h_moved = 0;

int is_square_under_attack(int row, int col, enum Color attacking_color) {
    int from_pos[2], to_pos[2];
    to_pos[0] = row;
    to_pos[1] = col;
    
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(board[i][j].color == attacking_color) {
                from_pos[0] = i;
                from_pos[1] = j;
                enum Type original_type = board[row][col].type;
                enum Color original_color = board[row][col].color;
                board[row][col].color = (attacking_color == WHITE) ? BLACK : WHITE;
                int can_attack = is_move_legal(from_pos, to_pos);
                // Restore the original piece
                board[row][col].type = original_type;
                board[row][col].color = original_color;
                if(can_attack) return 1;
            }
        }
    }
    return 0;
}

int is_in_check(enum Color color) {
    int king_row = -1, king_col = -1;
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(board[i][j].type == KING && board[i][j].color == color) {
                king_row = i;
                king_col = j;
                break;
            }
        }
        if(king_row != -1) break;
    }
    
    return is_square_under_attack(king_row, king_col, (color == WHITE) ? BLACK : WHITE);
}

int move_causes_check(int *from_pos, int *to_pos, enum Color moving_color) {
    struct Piece from_piece = board[from_pos[0]][from_pos[1]];
    struct Piece to_piece = board[to_pos[0]][to_pos[1]];
    
    board[to_pos[0]][to_pos[1]] = from_piece;
    board[from_pos[0]][from_pos[1]].type = NONE;
    board[from_pos[0]][from_pos[1]].color = NOCOLOR;
    
    int causes_check = is_in_check(moving_color);
    
    board[from_pos[0]][from_pos[1]] = from_piece;
    board[to_pos[0]][to_pos[1]] = to_piece;
    
    return causes_check;
}

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

	// En passant is missing

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

int queen_logic(int *from_pos, int *to_pos) {
    return rook_logic(from_pos, to_pos) || bishop_logic(from_pos, to_pos);
}

int king_logic(int *from_pos, int *to_pos) {
    // Normal king movement
    int dx = abs(to_pos[0] - from_pos[0]);
    int dy = abs(to_pos[1] - from_pos[1]);
    
    // one square movement
    if(dx <= 1 && dy <= 1 && (dx != 0 || dy != 0)) {
        return 1;
    }
    
    // Castling
    if(dx == 0 && dy == 2 && from_pos[0] == (board[from_pos[0]][from_pos[1]].color == WHITE ? 0 : 7)) {
        // Check if king has moved
        if(board[from_pos[0]][from_pos[1]].color == WHITE && white_king_moved) return 0;
        if(board[from_pos[0]][from_pos[1]].color == BLACK && black_king_moved) return 0;
        
        // Kingside castling
        if(to_pos[1] == from_pos[1] + 2) {
            // Check if rook has moved
            if(board[from_pos[0]][from_pos[1]].color == WHITE && white_rook_h_moved) return 0;
            if(board[from_pos[0]][from_pos[1]].color == BLACK && black_rook_h_moved) return 0;
            
            // Check if path is clear
            if(board[from_pos[0]][from_pos[1]+1].type != NONE || 
               board[from_pos[0]][from_pos[1]+2].type != NONE) return 0;
            
            // Check if king is in check or passes through check
            if(is_in_check(board[from_pos[0]][from_pos[1]].color)) return 0;
            
            int temp_pos[2] = {from_pos[0], from_pos[1]+1};
            if(is_square_under_attack(temp_pos[0], temp_pos[1], 
               board[from_pos[0]][from_pos[1]].color == WHITE ? BLACK : WHITE)) return 0;
            
            // Move rook
            board[from_pos[0]][from_pos[1]+1].type = CASTLE;
            board[from_pos[0]][from_pos[1]+1].color = board[from_pos[0]][from_pos[1]].color;
            board[from_pos[0]][7].type = NONE;
            board[from_pos[0]][7].color = NOCOLOR;
            return 1;
        }
        // Queenside castling
        else if(to_pos[1] == from_pos[1] - 2) {
            // Check if rook has moved
            if(board[from_pos[0]][from_pos[1]].color == WHITE && white_rook_a_moved) return 0;
            if(board[from_pos[0]][from_pos[1]].color == BLACK && black_rook_a_moved) return 0;
            
            // Check if path is clear
            if(board[from_pos[0]][from_pos[1]-1].type != NONE || 
               board[from_pos[0]][from_pos[1]-2].type != NONE ||
               board[from_pos[0]][from_pos[1]-3].type != NONE) return 0;
            
            // Check if king is in check or passes through check
            if(is_in_check(board[from_pos[0]][from_pos[1]].color)) return 0;
            
            int temp_pos[2] = {from_pos[0], from_pos[1]-1};
            if(is_square_under_attack(temp_pos[0], temp_pos[1], 
               board[from_pos[0]][from_pos[1]].color == WHITE ? BLACK : WHITE)) return 0;
            
            // Move rook
            board[from_pos[0]][from_pos[1]-1].type = CASTLE;
            board[from_pos[0]][from_pos[1]-1].color = board[from_pos[0]][from_pos[1]].color;
            board[from_pos[0]][0].type = NONE;
            board[from_pos[0]][0].color = NOCOLOR;
            return 1;
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

    if(piece == QUEEN){
        if(queen_logic(from_pos, to_pos)){
            return 1;
        }
    }

    if(piece == KING){
        if(king_logic(from_pos, to_pos)){
            return 1;
        }
    }

    return 0;
}



void move(char *from, char *to){
    int* from_pos = chess_notation_decoder(from);
    int* to_pos = chess_notation_decoder(to);

    // Check if it's the right player's turn
    if(board[from_pos[0]][from_pos[1]].color != current_turn) {
        printf("It's %s's turn!\n", current_turn == WHITE ? "White" : "Black");
        return;
    }

    // Check if the move is legal
    if(is_move_legal(from_pos, to_pos)) {
        // Check if the move would put/leave the king in check
        if(move_causes_check(from_pos, to_pos, current_turn)) {
            printf("That move would put/leave your king in check!\n");
            return;
        }

        // Track piece movement for castling
        if(board[from_pos[0]][from_pos[1]].type == KING) {
            if(board[from_pos[0]][from_pos[1]].color == WHITE) white_king_moved = 1;
            else black_king_moved = 1;
        }
        else if(board[from_pos[0]][from_pos[1]].type == CASTLE) {
            if(from_pos[0] == 0 && from_pos[1] == 0) white_rook_a_moved = 1;
            else if(from_pos[0] == 0 && from_pos[1] == 7) white_rook_h_moved = 1;
            else if(from_pos[0] == 7 && from_pos[1] == 0) black_rook_a_moved = 1;
            else if(from_pos[0] == 7 && from_pos[1] == 7) black_rook_h_moved = 1;
        }

        // Make the move
        board[to_pos[0]][to_pos[1]].type = board[from_pos[0]][from_pos[1]].type;
        board[to_pos[0]][to_pos[1]].color = board[from_pos[0]][from_pos[1]].color;
        board[from_pos[0]][from_pos[1]].type = NONE;
        board[from_pos[0]][from_pos[1]].color = NOCOLOR;

        // Check for pawn promotion
        if(board[to_pos[0]][to_pos[1]].type == PAWN) {
            if((current_turn == WHITE && to_pos[0] == 7) || 
               (current_turn == BLACK && to_pos[0] == 0)) {
                printf("Pawn promotion! Enter promotion piece (Q/R/B/N): ");
                char promotion;
                scanf("%c", &promotion);
                getchar(); // consume newline
                
                switch(promotion) {
                    case 'Q': board[to_pos[0]][to_pos[1]].type = QUEEN; break;
                    case 'R': board[to_pos[0]][to_pos[1]].type = CASTLE; break;
                    case 'B': board[to_pos[0]][to_pos[1]].type = BISHOP; break;
                    case 'N': board[to_pos[0]][to_pos[1]].type = HORSE; break;
                    default: board[to_pos[0]][to_pos[1]].type = QUEEN; break;
                }
            }
        }

        // Switch turns
        current_turn = (current_turn == WHITE) ? BLACK : WHITE;

        // Check if the opponent is in check
        if(is_in_check(current_turn)) {
            // Check if it's checkmate
            int has_legal_move = 0;
            int test_from[2], test_to[2];
            
            // Try every possible move for the current player
            for(int i = 0; i < 8 && !has_legal_move; i++) {
                for(int j = 0; j < 8 && !has_legal_move; j++) {
                    if(board[i][j].color == current_turn) {
                        test_from[0] = i;
                        test_from[1] = j;
                        
                        for(int k = 0; k < 8 && !has_legal_move; k++) {
                            for(int l = 0; l < 8 && !has_legal_move; l++) {
                                test_to[0] = k;
                                test_to[1] = l;
                                
                                if(is_move_legal(test_from, test_to) && 
                                   !move_causes_check(test_from, test_to, current_turn)) {
                                    has_legal_move = 1;
                                }
                            }
                        }
                    }
                }
            }
            
            if(!has_legal_move) {
                printf("Checkmate! %s wins!\n", current_turn == WHITE ? "Black" : "White");
                exit(0);
            } else {
                printf("Check!\n");
            }
        }
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
        printf("%s's turn\n", current_turn == WHITE ? "White" : "Black");
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

    free(from_str);
    free(to_str);
}





int main() {

    game_loop();


};
