#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;


const int boxes = 8;
const int squaresize = 100;
char board[boxes][boxes];
int firstlyselectedrow = -1;
int firstlyselectedcol = -1;
bool selectiondone = false;
bool whiteTurn = true;
bool gameover = false;
int whitescore = 0;
int blackscore = 0;

Texture wpTex, wrTex, wknTex, wbTex, wqTex, wkTex;
Texture bpTex, brTex, bknTex, bbTex, bqTex, bkTex;


Sprite wp[8], bp[8];
Sprite wr, wr2, wkn, wkn2, wb, wb2, wq, wk;
Sprite br, br2, bkn, bkn2, bb, bb2, bq, bk;

// ------------------- Helper functions -------------------
int absInt(int num)
{ 
    if (num < 0)
    {
        num = -num;
    }
    return num;
}

bool isWhite(char p) 
{
    bool flag = false;
    if (p >= 'A' && p <= 'Z')
    {
        flag = true;
    }
    return flag;
}


bool isBlack(char p) 
{
    bool flag = false;
    if (p >= 'a' && p <= 'z')
    {
        flag = true;
    }
    return flag;
}

int SCORES(char p)
{
    if (p == 'P' || p == 'p')
    {
        return 1;
    }
    if (p == 'N' || p == 'n')
    {
        return 3;
    }
    if (p == 'B' || p == 'b')
    {
        return 3;
    }
    if (p == 'R' || p == 'r')
    {
        return 5;
    }
    if (p == 'Q' || p == 'q')
    {
        return 9;
    }
    return 0;
}

//-------------------------------------------------------------FUNDAMENTAL FUNCTIONS---------------------------------------------------------------------------
void initializeBoard()
{
    const char whitePieces[8] = { 'R','N','B','Q','K','B','N','R' };
    const char blackPieces[8] = { 'r','n','b','q','k','b','n','r' };

    for (int r = 0; r < boxes; r++)
    {
        for (int c = 0; c < boxes; c++)
        {
            board[r][c] = '.';
        }
    }

    for (int c = 0; c < boxes; c++) 
    {
        board[6][c] = 'P';
        board[1][c] = 'p';
        board[7][c] = whitePieces[c];
        board[0][c] = blackPieces[c];
    }
}


bool striaghtpath(int sr, int sc, int dr, int dc)
{
    bool flag = true; 

    if (sr == dr) 
    {
        int step;
        if (dc > sc)
        {
            step = 1;
        }
        else
        {
            step = -1;
        }

        int i = sc + step;
        while (i != dc && flag) 
        {
            if (board[sr][i] != '.')
            {
                flag = false;
            }
            i += step;
        }
    }
    else if (sc == dc) 
    {
        int step;
        if (dr > sr)
        {
            step = 1;
        }
        else
        {
            step = -1;
        }

        int j = sr + step;
        while (j != dr && flag) 
        {
            if (board[j][sc] != '.')
            {
                flag = false;
            }
            j += step;
        }
    }
    else 
    {
        flag = false;
    }

    return flag; 
}



bool diagonalpath(int sr, int sc, int dr, int dc)
{
    bool flag = true; 
    int rowdiff = dr - sr;
    int coldiff = dc - sc;

    
    if (absInt(rowdiff) != absInt(coldiff))
    {
        flag = false; 
    }
    else
    {
        int stepR, stepC;
        if (dr > sr)
        {
            stepR = 1;
        }
        else
        {
            stepR = -1;
        }

        if (dc > sc)
        {
            stepC = 1;
        }
        else
        {
            stepC = -1;
        }

        int r = sr + stepR;
        int c = sc + stepC;

      
        while (r != dr && c != dc && flag)
        {
            if (board[r][c] != '.')
            {
                flag = false;
            }

            r += stepR;
            c += stepC;
        }
    }

    return flag; 
}


// ------------------- Piece validators -------------------
bool rook(int sr, int sc, int dr, int dc)
{
    char piece = board[sr][sc], target = board[dr][dc];
    bool flag = true;
    if ((isWhite(piece) && isWhite(target)) || (isBlack(piece) && isBlack(target)))
    {
        flag = false;
    }
    if (!(sr == dr || sc == dc))
    {
        flag = false;
    }
    if (!striaghtpath(sr, sc, dr, dc))
    {
        flag = false;
    }
    return flag;
}

bool bishop(int sr, int sc, int dr, int dc)
{
    char piece = board[sr][sc], target = board[dr][dc];
    bool flag = true;
    if ((isWhite(piece) && isWhite(target)) || (isBlack(piece) && isBlack(target)))
    {
        flag = false;
    }
    if (absInt(dr - sr) != absInt(dc - sc))
    {
        flag = false;
    }
    if (!diagonalpath(sr, sc, dr, dc))
    { 
        flag = false;
    }
    return flag;
}

bool knight(int sr, int sc, int dr, int dc)
{
    bool flag = true;   

    char piece = board[sr][sc];
    char target = board[dr][dc];

   
    if ((isWhite(piece) && isWhite(target)) ||(isBlack(piece) && isBlack(target)))
    {
       flag = false;
    }

    
    if (flag == true)
    {
        int rowdiff = absInt(dr - sr);
        int coldiff = absInt(dc - sc);

        if (!((rowdiff == 2 && coldiff == 1) || (rowdiff == 1 && coldiff == 2)))
        {
           flag = false;
        }
    }

  
    return flag;
}


bool Queen(int sr, int sc, int dr, int dc) 
{
     bool flag = false;

     if (rook(sr, sc, dr, dc) || bishop(sr, sc, dr, dc))
     {
         flag = true;
     }
   
     return flag;
}

bool King(int sr, int sc, int dr, int dc)
{
    bool flag= true;   

    char piece = board[sr][sc];
    char target = board[dr][dc];

    
    if ((isWhite(piece) && isWhite(target)) ||
        (isBlack(piece) && isBlack(target)))
    {
        flag= false;
    }

    
    if (flag == true)
    {
        int rD = absInt(dr - sr);
        int cD = absInt(dc - sc);

        if (!(rD <= 1 && cD <= 1))
        {
            flag= false;
        }
    }

 
    return flag;
}

bool Pawn(int sr, int sc, int dr, int dc)
{
    bool flag = false;  

    char piece = board[sr][sc];
    char target = board[dr][dc];

  
    if (piece == 'P')
    {
       
        if (dc == sc && dr == sr - 1 && target == '.')
        {
            flag = true;
        }

        
        else if (dc == sc && sr == 6 && dr == 4 && board[5][sc] == '.' && target == '.')
        {
            flag = true;
        }

        
        else if ((dc == sc - 1 || dc == sc + 1) && dr == sr - 1 && isBlack(target))
        {
            flag = true;
        }
    }

    
    else if (piece == 'p')
    {
        
        if (dc == sc && dr == sr + 1 && target == '.')
        {
            flag = true;
        }

     
        else if (dc == sc && sr == 1 && dr == 3 && board[2][sc] == '.' && target == '.')
        {
            flag = true;
        }

       
        else if ((dc == sc - 1 || dc == sc + 1) && dr == sr + 1 && isWhite(target))
        {
            flag = true;
        }
    }

    return flag; 
}

bool Validmoves(int sr, int sc, int dr, int dc)
{
    bool flag = false;    
    char piece = board[sr][sc];

    if (piece == 'R' || piece == 'r')
    {
        flag = rook(sr, sc, dr, dc);
    }
    else if (piece == 'B' || piece == 'b')
    {
        flag = bishop(sr, sc, dr, dc);
    }
    else if (piece == 'N' || piece == 'n')
    {
        flag = knight(sr, sc, dr, dc);
    }
    else if (piece == 'Q' || piece == 'q')
    {
        flag = Queen(sr, sc, dr, dc);
    }
    else if (piece == 'K' || piece == 'k')
    {
        flag = King(sr, sc, dr, dc);
    }
    else if (piece == 'P' || piece == 'p')
    {
        flag = Pawn(sr, sc, dr, dc);
    }

    return flag;  
}

bool isKingInCheck(bool whiteKing)
{
    bool flag = false;   

    char kingChar;
    if (whiteKing == true)
    {
        kingChar = 'K';
    }
    else
    {
        kingChar = 'k';
    }

    int kr = -1, kc = -1;
    bool found = false;

    
    for (int r = 0; r < boxes && found == false; r++)
    {
        for (int c = 0; c < boxes; c++)
        {
            if (found == false)
            {
                if (board[r][c] == kingChar)
                {
                    kr = r;
                    kc = c;
                    found = true;
                }
            }
        }
    }
    for (int r = 0; r < boxes; r++)
    {
        for (int c = 0; c < boxes; c++)
        {
            char piece = board[r][c];

            if (piece != '.')
            {
                if ((whiteKing && isBlack(piece)) || (!whiteKing && isWhite(piece)))
                {
                    if (Validmoves(r, c, kr, kc))
                    {
                        flag = true;
                    }
                }
            }
        }
    }
    

    return flag;
}

bool isMoveSafe(int sr, int sc, int dr, int dc, bool whiteTurn)
{
    bool flag = false;  

    char piece = board[sr][sc];
    char toPiece = board[dr][dc];

   
    board[dr][dc] = piece;
    board[sr][sc] = '.';

    // Checking if king is safe after move
    if (!isKingInCheck(whiteTurn))
    {
        flag = true;  
    }

   
    board[sr][sc] = piece;
    board[dr][dc] = toPiece;

    return flag;   
}


// ------------------- Compute legal moves -------------------
void computeLegalMovesForSelection(bool moves[boxes][boxes])
{
    for (int r = 0; r < boxes; r++)
    {
        for (int c = 0; c < boxes; c++)
        {
            moves[r][c] = false;
        }
    }

    if (!selectiondone)
    { return; }

    for (int r = 0; r < boxes; r++)
    {
        for (int c = 0; c < boxes; c++)
        {
            if (!(r == firstlyselectedrow && c == firstlyselectedcol))
            {
                if (Validmoves(firstlyselectedrow, firstlyselectedcol, r, c) && isMoveSafe(firstlyselectedrow, firstlyselectedcol, r, c, whiteTurn))
                {
                    moves[r][c] = true;
                }
            }
        }
    }
}


Texture& textureForChar(char p)
{
    if (p == 'P')
    {
        return wpTex;
    }
    if (p == 'R')
    {
        return wrTex;
    }
    if (p == 'N')
    {
        return wknTex;
    }
    if (p == 'B')
    {
        return wbTex;
    }
    if (p == 'Q')
    {
        return wqTex;
    }
    if (p == 'K')
    {
        return wkTex;
    }
    if (p == 'p')
    {
        return bpTex;
    }
    if (p == 'r')
    {
        return brTex;
    }
    if (p == 'n')
    { 
        return bknTex;
    }
    if (p == 'b')
    {
        return bbTex;
    }
    if (p == 'q') 
    {
        return bqTex;
    }
    if (p == 'k') 
    {
        return bkTex; 
    }
 
}


void drawBoard(RenderWindow& window)
{
   

    for (int r = 0; r < boxes; r++)
    {
        for (int c = 0; c < boxes; c++)
        {
            RectangleShape shape(Vector2f(squaresize, squaresize));
            shape.setPosition(c * squaresize, r * squaresize);
            if ((r + c) % 2 == 0)
            {
                shape.setFillColor(Color(222, 184, 135));
            } 
            else
            {
                shape.setFillColor(Color(139, 69, 19));
            } 

            window.draw(shape);
        }
    }
    Font font;
    font.loadFromFile("images/abcd.ttf");
   
    for (int r = 0; r < boxes; r++)
    {
        Text rank(to_string(boxes - r), font, 20);
        rank.setFillColor(Color::Black);
        rank.setPosition(2, r * squaresize + 5);
        window.draw(rank);
    }

   
    for (int c = 0; c < boxes; c++) 
    {
        char letter = 'a' + c;
        Text file(string(1,letter), font, 20);
        file.setFillColor(Color::Black);
        file.setPosition(c * squaresize + squaresize - 20, boxes * squaresize - 20);
        window.draw(file);
    }
}

void drawPieces(RenderWindow& window)
{
    for (int r = 0; r < boxes; r++)
    {
        for (int c = 0; c < boxes; c++)
        {
            char piece = board[r][c];
            if (piece != '.')
            {
                Sprite spr;
                spr.setTexture(textureForChar(piece));
                spr.setPosition(c * squaresize + 15, r * squaresize + 15);
                spr.setScale(0.2f, 0.2f);
                window.draw(spr);
            }
        }
    }
}

void drawHighlights(RenderWindow& window, bool moves[boxes][boxes])
{
    if (selectiondone)
    {
       
        RectangleShape selected(Vector2f(squaresize - 4, squaresize - 4));
        selected.setPosition(firstlyselectedcol * squaresize + 2, firstlyselectedrow * squaresize + 2);
        selected.setFillColor(Color(0, 0, 0, 0));
        selected.setOutlineThickness(3);
        selected.setOutlineColor(Color::Yellow);
        window.draw(selected);

       
        for (int r = 0; r < boxes; r++)
        {
            for (int c = 0; c < boxes; c++)
            {
                if (moves[r][c] == true)
                {
                    RectangleShape highlight(Vector2f(squaresize - 6, squaresize - 6));
                    highlight.setPosition(c * squaresize + 3, r * squaresize + 3);

                    if (board[r][c] != '.')
                    {
                        highlight.setFillColor(Color(220, 20, 60, 180)); 
                    }
                    else
                    {
                        highlight.setFillColor(Color(144, 238, 144, 180)); 
                    }

                    window.draw(highlight);
                }
            }
        }
    }
}



bool hasAnyLegalMove(bool whitePlayer)
{
    bool flag = false; 

    for (int sr = 0; sr < boxes; sr++)
    {
        for (int sc = 0; sc < boxes; sc++)
        {
            char piece = board[sr][sc];

            if (piece != '.')
            {
                if ((whitePlayer && isWhite(piece)) || (!whitePlayer && isBlack(piece)))
                {
                    for (int dr = 0; dr < boxes; dr++)
                    {
                        for (int dc = 0; dc < boxes; dc++)
                        {

                            if (sr != dr || sc != dc)
                            {
                                if (Validmoves(sr, sc, dr, dc) && isMoveSafe(sr, sc, dr, dc, whitePlayer))
                                {
                                    flag = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return flag;
}








// -------------------------------------------------------------------Main ------------------------------------------------------------------------------------------------------
int main()
{
    initializeBoard();

    
    if (!wpTex.loadFromFile("images/wp.png") ||!wrTex.loadFromFile("images/wr.png") || !wknTex.loadFromFile("images/wkn.png") ||!wbTex.loadFromFile("images/wb.png") ||
        !wqTex.loadFromFile("images/wq.png") ||!wkTex.loadFromFile("images/wk.png") ||!bpTex.loadFromFile("images/bp.png") ||!brTex.loadFromFile("images/br.png") ||
        !bknTex.loadFromFile("images/bkn.png") ||!bbTex.loadFromFile("images/bb.png") ||!bqTex.loadFromFile("images/bq.png") ||!bkTex.loadFromFile("images/bk.png")) 
    {
            cout << "Error loading textures!\n";
            return 0;
    }

    RenderWindow window(VideoMode(squaresize * boxes, squaresize * boxes), "Chess Game By Muhammad Abdullah", Style::Titlebar | Style::Close);

    bool moves[boxes][boxes] = { false };

    while(window.isOpen())
    {
        Event e;
        while(window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                window.close();
            }

            if(e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) 
            {
                if (gameover)
                {
                    return 0;
                }
               
                int row = e.mouseButton.y / squaresize;
                int col = e.mouseButton.x / squaresize;
                if (row < 0)
                {
                    row = 0;
                }
                if (row > 7)
                {
                    row = 7;
                }                
                if (col < 0)
                {
                    col = 0;
                }
                if (col > 7)
                {
                    col = 7;
                }

                char selectedpiece = board[row][col];

                if (!selectiondone)
                {
                    if ((whiteTurn && isWhite(selectedpiece)) || (!whiteTurn && isBlack(selectedpiece)))
                    {
                        firstlyselectedrow = row;
                        firstlyselectedcol = col;
                        selectiondone = true;
                        computeLegalMovesForSelection(moves);
                        cout << "Selected " << selectedpiece << " at (" << boxes - row << "," << col + 1 << ")\n";
                    }
                }
                else
                {
                    if (row == firstlyselectedrow && col == firstlyselectedcol)
                    {
                        selectiondone = false;
                        firstlyselectedrow = firstlyselectedcol = -1;
                        for (int r = 0; r < 8; r++)
                        {
                            for (int c = 0; c < 8; c++)
                            {
                                
                                    moves[r][c] = false;
                            }
                        }
                    
                        cout << "Deselected.\n";
                    }
                    else if (moves[row][col] && firstlyselectedrow >= 0 && firstlyselectedcol>=0)
                    {
                        char piece = board[firstlyselectedrow][firstlyselectedcol];
                        char captured = board[row][col];
                        
                        board[row][col] = piece;
                        board[firstlyselectedrow][firstlyselectedcol] = '.';
                        if (captured != '.')
                        {
                            if (whiteTurn)   
                            {
                                whitescore += SCORES(captured);
                            }
                            else                      
                            {
                                blackscore += SCORES(captured);
                            }
                        }


                       // PAWN TO QUEEN 
                        if (piece == 'P' && row == 0)
                        {
                            board[row][col] = 'Q';
                        }
                        if (piece == 'p' && row == 7) 
                        { 
                            board[row][col] = 'q'; 
                        }

                        selectiondone = false;
                        firstlyselectedrow = firstlyselectedcol = -1;
                        for (int r = 0; r < 8; r++)
                        {
                            for (int c = 0; c < 8; c++)
                            {
                                moves[r][c] = false;
                            }
                        }

                        // Changing turn
                        whiteTurn = !whiteTurn;

                        // checking for check, checkmate and stalemate conditions
                        bool opponentKingInCheck = isKingInCheck(whiteTurn);
                        bool opponentHasMoves = hasAnyLegalMove(whiteTurn);

                        string player;
                        if (whiteTurn)
                        {
                            player = "White";
                        }
                        else
                        {
                            player = "Black";
                        }

                        if (opponentKingInCheck && !opponentHasMoves)
                        {
                            if (whiteTurn != true)
                            {
                                cout << "White wins by CHECKMATE!\n";

                            }
                            else
                            {
                                cout << "Black wins by CHECKMATE!\n";
                            }

                            gameover = true;
                            cout << "White Score: " << whitescore << " | Black Score: " << blackscore << endl;

                        }
                        else if (!opponentKingInCheck && !opponentHasMoves)
                        {
                            cout << "Game ends in STALEMATE!\n";
                              gameover = true;
                              cout << "White Score: " << whitescore << " | Black Score: " << blackscore << endl;

                        }
                        else if (opponentKingInCheck)
                        {
                            cout << player << " KING IS IN CHECK!\n";
                        }
                        else
                        {
                            cout << player << "'s turn.\n";
                        }


                    }
                    else
                    {
                        // select a different piece of same color
                        if ((whiteTurn && isWhite(selectedpiece)) || (!whiteTurn && isBlack(selectedpiece)))
                        {
                            firstlyselectedrow = row;
                            firstlyselectedcol = col;
                            selectiondone = true;
                            computeLegalMovesForSelection(moves);
                            cout << "Selected " << selectedpiece << " at (" << boxes - row << "," << col + 1 << ")\n";
                        }
                        else
                        {
                            cout << "Not a legal move.\n";
                        }
                    }
                }
            }


        }

        window.clear();
        drawBoard(window);
        drawHighlights(window, moves);
        drawPieces(window);
        window.display();
    }

    return 0;
}
