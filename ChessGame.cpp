#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <algorithm>
#include <set>
#include <conio.h>
#include <map>
#include <limits>
#include <windows.h>
#undef max
#undef min
using namespace std;

enum Spalva { BALTA, JUODA };

class Figura {
protected:
    Spalva spalva;
public:
    Figura(Spalva col) : spalva(col) {}
    virtual ~Figura() {}
    virtual char getSymbol() const = 0;
    virtual Figura* clone() const = 0;
    Spalva getSpalva() const { return spalva; }
    virtual bool canMove(int fromX, int fromY, int toX, int toY, const vector<vector<Figura*>>& lent) const = 0;
};

class Valdove : public Figura {
public:
    Valdove(Spalva col) : Figura(col) {}
    char getSymbol() const override { return 'Q'; }
    Figura* clone() const override { return new Valdove(spalva); }
    bool canMove(int fromX, int fromY, int toX, int toY, const vector<vector<Figura*>>& lent) const override {
        int dx = toX - fromX, dy = toY - fromY;
        if (dx == 0 || dy == 0 || abs(dx) == abs(dy)) {
            int stepX = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
            int stepY = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);
            int x = fromX + stepX, y = fromY + stepY;
            while (x != toX || y != toY) {
                if (lent[x][y] != nullptr) return false;
                x += stepX; y += stepY;
            }
            return (lent[toX][toY] == nullptr || lent[toX][toY]->getSpalva() != spalva);
        }
        return false;
    }
};

class Bokstas : public Figura {
    bool hasMoved;
public:
    Bokstas(Spalva col) : Figura(col), hasMoved(false) {}
    char getSymbol() const override { return 'R'; }
    Figura* clone() const override { 
        Bokstas* newRook = new Bokstas(spalva);
        newRook->hasMoved = hasMoved;
        return newRook; 
    }
    bool canMove(int fromX, int fromY, int toX, int toY, const vector<vector<Figura*>>& lent) const override {
        if (fromX != toX && fromY != toY) return false;
        int dx = (toX > fromX) ? 1 : (toX < fromX ? -1 : 0);
        int dy = (toY > fromY) ? 1 : (toY < fromY ? -1 : 0);
        int x = fromX + dx, y = fromY + dy;
        while (x != toX || y != toY) {
            if (lent[x][y] != nullptr) return false;
            x += dx; y += dy;
        }
        return (lent[toX][toY] == nullptr || lent[toX][toY]->getSpalva() != spalva);
    }
    void markMoved() { hasMoved = true; }
    bool getMoved() const { return hasMoved; }
};

class Karalius : public Figura {
    bool hasMoved;
public:
    Karalius(Spalva col) : Figura(col), hasMoved(false) {}
    char getSymbol() const override { return 'K'; }
    Figura* clone() const override { 
        Karalius* newKing = new Karalius(spalva);
        newKing->hasMoved = hasMoved;
        return newKing; 
    }
    bool canMove(int fromX, int fromY, int toX, int toY, const vector<vector<Figura*>>& lent) const override {
        int dx = abs(fromX - toX), dy = abs(fromY - toY);
        if (max(dx, dy) == 1) {
            return (lent[toX][toY] == nullptr || lent[toX][toY]->getSpalva() != spalva);
        }
        
        if (!hasMoved && fromX == toX) {
            if (toY == fromY + 2) {
                if (lent[fromX][fromY+1] != nullptr || lent[fromX][fromY+2] != nullptr) return false;
                if (lent[fromX][7] == nullptr || lent[fromX][7]->getSymbol() != 'R' || 
                    static_cast<Bokstas*>(lent[fromX][7])->getMoved()) return false;
                return true;
            }
            if (toY == fromY - 2) {
                if (lent[fromX][fromY-1] != nullptr || lent[fromX][fromY-2] != nullptr || 
                    lent[fromX][fromY-3] != nullptr) return false;
                if (lent[fromX][0] == nullptr || lent[fromX][0]->getSymbol() != 'R' || 
                    static_cast<Bokstas*>(lent[fromX][0])->getMoved()) return false;
                return true;
            }
        }
        return false;
    }
    void markMoved() { hasMoved = true; }
    bool getMoved() const { return hasMoved; }
};

class Rikis : public Figura {
public:
    Rikis(Spalva col) : Figura(col) {}
    char getSymbol() const override { return 'B'; }
    Figura* clone() const override { return new Rikis(spalva); }
    bool canMove(int fromX, int fromY, int toX, int toY, const vector<vector<Figura*>>& lent) const override {
        int dx = abs(fromX - toX), dy = abs(fromY - toY);
        if (dx != dy) return false;
        int stepX = (toX > fromX) ? 1 : -1, stepY = (toY > fromY) ? 1 : -1;
        int x = fromX + stepX, y = fromY + stepY;
        while (x != toX) {
            if (lent[x][y] != nullptr) return false;
            x += stepX; y += stepY;
        }
        return (lent[toX][toY] == nullptr || lent[toX][toY]->getSpalva() != spalva);
    }
};

class Zirgas : public Figura {
public:
    Zirgas(Spalva col) : Figura(col) {}
    char getSymbol() const override { return 'N'; }
    Figura* clone() const override { return new Zirgas(spalva); }
    bool canMove(int fromX, int fromY, int toX, int toY, const vector<vector<Figura*>>& lent) const override {
        int dx = abs(fromX - toX), dy = abs(fromY - toY);
        if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2))
            return (lent[toX][toY] == nullptr || lent[toX][toY]->getSpalva() != spalva);
        return false;
    }
};

class Peske : public Figura {
public:
    Peske(Spalva col) : Figura(col) {}
    char getSymbol() const override { return 'P'; }
    Figura* clone() const override { return new Peske(spalva); }
    bool canMove(int fromX, int fromY, int toX, int toY, const vector<vector<Figura*>>& lent) const override {
        int dir = (spalva == BALTA) ? -1 : 1; 
        int startRow = (spalva == BALTA) ? 6 : 1;
        if (toX < 0 || toX >= 8 || toY < 0 || toY >= 8) return false;
        if (toX - fromX == dir && toY == fromY && !lent[toX][toY]) return true;
        if (fromX == startRow && toX - fromX == 2 * dir && toY == fromY && !lent[fromX + dir][fromY] && !lent[toX][toY]) return true;
        if (toX - fromX == dir && abs(toY - fromY) == 1 && lent[toX][toY] && lent[toX][toY]->getSpalva() != spalva) return true;
        return false;
    }
};

class SachmatuLenta {
private:
    vector<vector<Figura*>> lent;
    HANDLE hConsole;

    int getPieceValue(Figura* fig) const {
        if (!fig) return 0;
        static const map<char, int> pieceValues = {
            {'P', 1}, {'N', 3}, {'B', 3}, {'R', 5}, {'Q', 9}, {'K', 100}
        };
        char symbol = fig->getSymbol();
        auto it = pieceValues.find(symbol);
        return (it != pieceValues.end()) ? it->second : 0;
    }

    bool isSquareAttacked(int x, int y, Spalva attackerColor, const vector<vector<Figura*>>& board) const {
        for(int i = 0; i < 8; i++) for(int j = 0; j < 8; j++)
            if(board[i][j] && board[i][j]->getSpalva() == attackerColor)
                if(board[i][j]->canMove(i, j, x, y, board)) return true;
        return false;
    }

    bool isCenter(int x, int y) const { return (x >= 3 && x <= 4 && y >= 3 && y <= 4); }

    int staticExchangeEval(int fromX, int fromY, int toX, int toY, const vector<vector<Figura*>>& board) {
        if (!board[fromX][fromY] || !board[toX][toY]) return 0;
        int gain = getPieceValue(board[toX][toY]);
        Spalva defenderColor = board[fromX][fromY]->getSpalva();
        vector<vector<Figura*>> tempBoard = copyBoard(board);

        delete tempBoard[toX][toY];
        tempBoard[toX][toY] = tempBoard[fromX][fromY];
        tempBoard[fromX][fromY] = nullptr;

        if(isSquareAttacked(toX, toY, opposite(defenderColor), tempBoard))
            gain -= getPieceValue(tempBoard[toX][toY]);  

        for (auto& row : tempBoard) for (auto& p : row) delete p;
        return gain;
    }

    int evaluateBoard(const vector<vector<Figura*>>& board, Spalva us) const {
        int score = 0;
        for(int i=0; i<8; i++) for(int j=0; j<8; j++)
            if(board[i][j]) {
                int v = getPieceValue(board[i][j]);
                score += (board[i][j]->getSpalva()==us ? v : -v);
                if(isCenter(i,j)) {
                    char symbol = board[i][j]->getSymbol();
                    int centralityBonus = (symbol == 'N' || symbol == 'P') ? 2 : 1;
                    score += (board[i][j]->getSpalva() == us) ? centralityBonus : -centralityBonus;
                }
            }
        return score;
    }

    Spalva opposite(Spalva c) const { return c==BALTA ? JUODA : BALTA; }

    bool isKingInCheck(const vector<vector<Figura*>>& board, Spalva c) {
        int kingX = -1, kingY = -1;
        for(int i=0; i<8; i++) for(int j=0; j<8; j++)
            if(board[i][j] && board[i][j]->getSpalva() == c && board[i][j]->getSymbol() == 'K')
                { kingX = i; kingY = j; break; }
        if(kingX == -1) return false;
        return isSquareAttacked(kingX, kingY, opposite(c), board);
    }

    int minimax(const vector<vector<Figura*>>& board, int depth, Spalva turn, int alpha, int beta, Spalva us) {
        if(depth==0) return evaluateBoard(board, us);
        if(turn==us) {
            int maxEval = INT_MIN;
            for(int i=0; i<8; i++) for(int j=0; j<8; j++)
                if(board[i][j] && board[i][j]->getSpalva()==turn)
                    for(int x=0; x<8; x++) for(int y=0; y<8; y++)
                        if(board[i][j]->canMove(i,j,x,y,board)) {
                            if(board[x][y] && board[x][y]->getSpalva()!=turn)
                                if(staticExchangeEval(i,j,x,y,board) <= 0) continue;
                            vector<vector<Figura*>> temp = copyBoard(board);
                            delete temp[x][y]; temp[x][y] = temp[i][j]; temp[i][j] = nullptr;
                            int eval = minimax(temp, depth-1, opposite(turn), alpha, beta, us);
                            maxEval = max(maxEval, eval); alpha = max(alpha, eval);
                            for(auto &row : temp) for(auto &p : row) delete p;
                            if(beta <= alpha) goto out_max;
                        }
            out_max:
            return maxEval;
        } else {
            int minEval = INT_MAX;
            for(int i=0; i<8; i++) for(int j=0; j<8; j++)
                if(board[i][j] && board[i][j]->getSpalva()==turn)
                    for(int x=0; x<8; x++) for(int y=0; y<8; y++)
                        if(board[i][j]->canMove(i,j,x,y,board)) {
                            if(board[x][y] && board[x][y]->getSpalva()!=turn)
                                if(staticExchangeEval(i,j,x,y,board) <= 0) continue;
                            vector<vector<Figura*>> temp = copyBoard(board);
                            delete temp[x][y]; temp[x][y] = temp[i][j]; temp[i][j] = nullptr;
                            int eval = minimax(temp, depth-1, opposite(turn), alpha, beta, us);
                            minEval = min(minEval, eval); beta = min(beta, eval);
                            for(auto &row : temp) for(auto &p : row) delete p;
                            if(beta <= alpha) goto out_min;
                        }
            out_min:
            return minEval;
        }
    }

    vector<vector<Figura*>> copyBoard(const vector<vector<Figura*>>& board) const {
        vector<vector<Figura*>> newBoard(8, vector<Figura*>(8, nullptr));
        for(int i = 0; i < 8; i++) for(int j = 0; j < 8; j++)
            if(board[i][j]) newBoard[i][j] = board[i][j]->clone();
        return newBoard;
    }

    void movePieceInternal(vector<vector<Figura*>>& board, int fromX, int fromY, int toX, int toY) {
        if (!board[fromX][fromY]) return;
        delete board[toX][toY]; 
        board[toX][toY] = board[fromX][fromY];
        board[fromX][fromY] = nullptr;
    }

    int positionalBonus(int x, int y, Figura* fig) const {
        if (!fig) return 0;
        int bonus = 0;
        if (isCenter(x, y)) bonus += 1;
        char symbol = fig->getSymbol();
        if (symbol == 'P')
            bonus += (fig->getSpalva() == BALTA ? 6 - x : x - 1);
        if (symbol == 'N' && (x >= 2 && x <= 5 && y >= 2 && y <= 5))
            bonus += 1;
        return bonus;
    }

    Figura* transformPawn(Spalva spalva) {
        system("cls");
        cout << "\nPawn reached the end of the board! Choose transformation:\n";
        cout << "1. Queen (Q)\n2. Rook (R)\n3. Bishop (B)\n4. Knight (N)\n";
        int choice;
        for (;;) {
            cout << "Enter your choice (1-4): ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            switch(choice) {
                case 1: return new Valdove(spalva);
                case 2: return new Bokstas(spalva);
                case 3: return new Rikis(spalva);
                case 4: return new Zirgas(spalva);
                default: cout << "Invalid choice. Please try again.\n";
            }
        }
    }

public:
    SachmatuLenta() {
        lent.resize(8, vector<Figura*>(8, nullptr));
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        setupLenta();
    }
    ~SachmatuLenta() {
        for(auto &row : lent) for(auto &p : row) { delete p; p = nullptr; }
    }

    void setupLenta() {
        for(auto &row : lent) for(auto &p : row) { delete p; p = nullptr; }
        for(int j = 0; j < 8; j++) {
            lent[1][j] = new Peske(JUODA);
            lent[6][j] = new Peske(BALTA);
        }
        lent[0][0] = new Bokstas(JUODA); lent[0][7] = new Bokstas(JUODA);
        lent[7][0] = new Bokstas(BALTA); lent[7][7] = new Bokstas(BALTA);
        lent[0][1] = new Zirgas(JUODA); lent[0][6] = new Zirgas(JUODA);
        lent[7][1] = new Zirgas(BALTA); lent[7][6] = new Zirgas(BALTA);
        lent[0][2] = new Rikis(JUODA); lent[0][5] = new Rikis(JUODA);
        lent[7][2] = new Rikis(BALTA); lent[7][5] = new Rikis(BALTA);
        lent[0][3] = new Valdove(JUODA); lent[7][3] = new Valdove(BALTA);
        lent[0][4] = new Karalius(JUODA); lent[7][4] = new Karalius(BALTA);
    }
    
    void printLenta() const {
        system("cls"); 
        cout << "\n    a b c d e f g h\n  +-----------------+\n";
        for(int i = 0; i < 8; i++) {
            cout << 8-i << " | ";
            for(int j = 0; j < 8; j++) {
                WORD attributes = ((i + j) % 2 == 0 ? (10 << 4) : (8 << 4));
                if(lent[i][j])
                    attributes |= (lent[i][j]->getSpalva() == BALTA ? 15 : 0);
                SetConsoleTextAttribute(hConsole, attributes);
                cout << (lent[i][j] ? lent[i][j]->getSymbol() : ' ') << " ";
                SetConsoleTextAttribute(hConsole, 7);
            }
            cout << "| " << 8-i << endl;
        }
        cout << "  +-----------------+\n    a b c d e f g h\n\n";
    }

    bool movePiece(const string& from, const string& to, Spalva currentTurn) {
        if (from.length() != 2 || to.length() != 2) return false;
        int fromY = from[0] - 'a';
        int fromX = 8 - (from[1] - '0');
        int toY = to[0] - 'a';
        int toX = 8 - (to[1] - '0');
    
        if (fromX < 0 || fromX > 7 || fromY < 0 || fromY > 7 ||
            toX < 0 || toX > 7 || toY < 0 || toY > 7) {
            cout << "Invalid coordinates!" << endl;
            return false;
        }
    
        if (!lent[fromX][fromY]) {
            cout << "No piece at starting position!" << endl;
            return false;
        }
    
        if (lent[fromX][fromY]->getSpalva() != currentTurn) {
            cout << "You can only move your own pieces!" << endl;
            return false;
        }
    
        if (!lent[fromX][fromY]->canMove(fromX, fromY, toX, toY, lent)) {
            cout << "Invalid move for this piece!" << endl;
            return false;
        }
        Figura* capturedPiece = lent[toX][toY];
        if (lent[fromX][fromY]->getSymbol() == 'K') {
            Karalius* king = static_cast<Karalius*>(lent[fromX][fromY]);
            if (!king->getMoved()) {
                if (toY == fromY + 2) {
                    lent[fromX][fromY + 1] = lent[fromX][7];
                    lent[fromX][7] = nullptr;
                    static_cast<Bokstas*>(lent[fromX][fromY + 1])->markMoved();
                }
                else if (toY == fromY - 2) {
                    lent[fromX][fromY - 1] = lent[fromX][0];
                    lent[fromX][0] = nullptr;
                    static_cast<Bokstas*>(lent[fromX][fromY - 1])->markMoved();
                }
            }
            king->markMoved();
        }
        else if (lent[fromX][fromY]->getSymbol() == 'R') {
            static_cast<Bokstas*>(lent[fromX][fromY])->markMoved();
        }
        lent[toX][toY] = lent[fromX][fromY];
        lent[fromX][fromY] = nullptr;

        if (isKingInCheck(lent, lent[toX][toY]->getSpalva())) {
            lent[fromX][fromY] = lent[toX][toY];
            lent[toX][toY] = capturedPiece;
            if (lent[fromX][fromY]->getSymbol() == 'K') {
                if (toY == fromY + 2) { lent[fromX][7] = lent[fromX][fromY + 1]; lent[fromX][fromY + 1] = nullptr; }
                else if (toY == fromY - 2) { lent[fromX][0] = lent[fromX][fromY - 1]; lent[fromX][fromY - 1] = nullptr; }
            }
            cout << "Move would put your king in check!" << endl;
            return false;
        }
        if (lent[toX][toY]->getSymbol() == 'P') {
            if ((lent[toX][toY]->getSpalva() == BALTA && toX == 0) || (lent[toX][toY]->getSpalva() == JUODA && toX == 7)) {
                delete lent[toX][toY];
                lent[toX][toY] = transformPawn(lent[toX][toY]->getSpalva());
            }
        }
        delete capturedPiece;
        return true;
    }

    string suggestMove(Spalva turn) {
        int bestScore = INT_MIN;
        string bestFrom, bestTo;
        bool foundMove = false;
    
        bool isOpening = true;
        for (int j = 2; j < 6; j++) {
            for (int i = 0; i < 8; i++) {
                if (lent[j][i] != nullptr) {
                    isOpening = false;
                    break;
                }
            }
            if (!isOpening) break;
        }
    
        if (isOpening) {
            if (turn == BALTA) {
                if (lent[6][4] && lent[6][4]->getSymbol() == 'P' && 
                    !lent[5][4] && !lent[4][4] && 
                    lent[6][4]->canMove(6, 4, 4, 4, lent)) {
                    return "e2 e4";
                }
                if (lent[6][3] && lent[6][3]->getSymbol() == 'P' && 
                    !lent[5][3] && !lent[4][3] && 
                    lent[6][3]->canMove(6, 3, 4, 3, lent)) {
                    return "d2 d4";
                }
            }
            else {
                if (lent[1][4] && lent[1][4]->getSymbol() == 'P' && 
                    !lent[2][4] && !lent[3][4] && 
                    lent[1][4]->canMove(1, 4, 3, 4, lent)) {
                    return "e7 e5";
                }
                if (lent[1][3] && lent[1][3]->getSymbol() == 'P' && 
                    !lent[2][3] && !lent[3][3] && 
                    lent[1][3]->canMove(1, 3, 3, 3, lent)) {
                    return "d7 d5";
                }
            }
        }
    
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (!lent[i][j] || lent[i][j]->getSpalva() != turn) continue;
                
                for (int x = 0; x < 8; x++) {
                    for (int y = 0; y < 8; y++) {
                        if (i == x && j == y) continue;
                        if (!lent[i][j]->canMove(i, j, x, y, lent)) continue;
    
                        auto tempBoard = copyBoard(lent);
                        movePieceInternal(tempBoard, i, j, x, y);
                        
                        if (isKingInCheck(tempBoard, turn)) {
                            for (auto &row : tempBoard) for (auto &p : row) delete p;
                            continue;
                        }
    
                        int score = 0;
    
                        if (lent[x][y]) {
                            if (lent[x][y]->getSpalva() != turn) {
                                score += getPieceValue(lent[x][y]) * 10;
                            } else {
                                for (auto &row : tempBoard) for (auto &p : row) delete p;
                                continue;
                            }
                        }
    
                        if ((x == 3 || x == 4) && (y == 3 || y == 4)) {
                            score += 8;
                        }
                        else if (x >= 2 && x <= 5 && y >= 2 && y <= 5) {
                            score += 4;
                        }
    
                        if ((turn == BALTA && i == 6) || (turn == JUODA && i == 1)) {
                            score += 3;
                        }
    
                        switch (lent[i][j]->getSymbol()) {
                            case 'N': 
                                if (x >= 2 && x <= 5 && y >= 2 && y <= 5) score += 4;
                                if (isOpening && ((turn == BALTA && i == 7) || (turn == JUODA && i == 0))) {
                                    score += 5;
                                }
                                break;
                            case 'B': 
                                if (abs(x - i) >= 2) score += 2;
                                if (isOpening && ((turn == BALTA && i == 7) || (turn == JUODA && i == 0))) {
                                    score += 4;
                                }
                                break;
                            case 'R': 
                                if (x == 0 || x == 7) score += 2;
                                break;
                            case 'Q': 
                                if (isOpening) score -= 10;
                                break;
                            case 'P': 
                                score += (turn == BALTA ? 6 - x : x - 1);
                                if (y >= 2 && y <= 5) score += 2;
                                if (y == 3 || y == 4) score += 3;
                                break;
                        }
    
                        if (y == 0 || y == 7) {
                            score -= 2;
                        }
    
                        foundMove = true;
                        if (score > bestScore) {
                            bestScore = score;
                            bestFrom = string(1, 'a' + j) + to_string(8 - i);
                            bestTo = string(1, 'a' + y) + to_string(8 - x);
                        }
    
                        for (auto &row : tempBoard) for (auto &p : row) delete p;
                    }
                }
            }
        }
    
        if (!foundMove) {
            return "Nėra tinkamo ėjimo";
        }
        return bestFrom + " " + bestTo;
    }
    
    void showAvailableMoves(const string& pos) {
        if(pos.length() != 2) return;
        int y = pos[0] - 'a', x = 8 - (pos[1] - '0');
        if(x < 0 || x > 7 || y < 0 || y > 7 || !lent[x][y]) {
            cout << "No piece at position " << pos << endl; return;
        }
        vector<string> moves;
        for(int i = 0; i < 8; i++) for(int j = 0; j < 8; j++)
            if(lent[x][y]->canMove(x, y, i, j, lent)) {
                auto tempBoard = copyBoard(lent);
                movePieceInternal(tempBoard, x, y, i, j);
                if(!isKingInCheck(tempBoard, lent[x][y]->getSpalva()))
                    moves.push_back(string(1, 'a'+j) + to_string(8-i));
                for(auto &row : tempBoard) for(auto &p : row) delete p;
            }
        if(moves.empty()) cout << "No valid moves for piece at " << pos << endl;
        else {
            cout << "Available moves for " << pos << ": ";
            for(size_t i = 0; i < moves.size(); i++) {
                cout << moves[i]; if(i < moves.size() - 1) cout << ", ";
            }
            cout << endl;
        }
    }

    
    bool isGameOver(Spalva turn, bool& isCheckmate) {
        isCheckmate = false;
        bool inCheck = isKingInCheck(lent, turn);
        
        for (int fromI = 0; fromI < 8; fromI++) {
            for (int fromJ = 0; fromJ < 8; fromJ++) {
                if (!lent[fromI][fromJ] || lent[fromI][fromJ]->getSpalva() != turn) 
                    continue;
                
                for (int toI = 0; toI < 8; toI++) {
                    for (int toJ = 0; toJ < 8; toJ++) {
                        if (fromI == toI && fromJ == toJ) 
                            continue;
                            
                        if (!lent[fromI][fromJ]->canMove(fromI, fromJ, toI, toJ, lent))
                            continue;
                            
                        auto tempBoard = copyBoard(lent);
                        Figura* capturedPiece = tempBoard[toI][toJ];
                        tempBoard[toI][toJ] = tempBoard[fromI][fromJ];
                        tempBoard[fromI][fromJ] = nullptr;
                        
                        bool stillInCheck = isKingInCheck(tempBoard, turn);
                        
                        for (auto& row : tempBoard)
                            for (auto& p : row)
                                delete p;
                                
                        if (!stillInCheck)
                            return false; 
                    }
                }
            }
        }
        
        isCheckmate = inCheck;
        return true;
    }
};

void displayHelp() {
    cout << "\n==== Chess Game Help ====\n";
    cout << "Commands:\n";
    cout << "- Enter moves as 'e2 e4' (from-to notation)\n";
    cout << "- 'help' - Show this help\n";
    cout << "- 'hint' - Show suggested move\n";
    cout << "- 'moves e2' - Show available moves for piece at e2\n";
    cout << "- 'exit' - Quit the game\n";
    cout << "========================\n\n";
}

int main() {
    try {
        SetConsoleCP(65001);
        SetConsoleOutputCP(65001);
        SachmatuLenta lenta;
        string command;
        Spalva currentTurn = BALTA; 
        bool gameRunning = true;
        cout << "Welcome to Chess! Type 'help' for commands.\n";
        lenta.printLenta();
        while (gameRunning) {
            try {
                cout << "Type 'help' for commands.\n";
                cout << (currentTurn == BALTA ? "White" : "Black") << "'s turn. Enter command: ";
                getline(cin, command);
                if (command == "exit") {
                    cout << "Thanks for playing!" << endl;
                    gameRunning = false;
                } 
                else if (command == "help") {
                    displayHelp();
                    cout << "Press any key to continue..."; _getch(); lenta.printLenta();
                } 
                else if (command == "hint") {
                    try {
                        cout << "Calculating hint... please wait." << endl;
                        string suggestion = lenta.suggestMove(currentTurn);
                        if (suggestion == "Nėra tinkamo ėjimo") {
                            cout << "No legal moves for this player. The game is likely over." << endl;
                        } else {
                            cout << "Suggested move: " << suggestion << endl;
                        }
                        cout << "Press any key to continue...";
                        _getch();
                        lenta.printLenta();
                    } catch (const exception& e) {
                        cout << "Error generating hint: " << e.what() << endl;
                        cout << "Press any key to continue...";
                        _getch();
                        lenta.printLenta();
                    }
                }
                else if (command.substr(0, 5) == "moves" && command.length() == 8) {
                    lenta.showAvailableMoves(command.substr(6, 2));
                    cout << "Press any key to continue..."; _getch(); lenta.printLenta();
                } 
                else if (command.length() == 5 && command[2] == ' ') {
                    string from = command.substr(0, 2), to = command.substr(3, 2);
                    if (lenta.movePiece(from, to, currentTurn)) {
                        lenta.printLenta();
                        
                        Spalva nextTurn = (currentTurn == BALTA) ? JUODA : BALTA;
                        bool isCheckmate = false;
                        
                        if (lenta.isGameOver(nextTurn, isCheckmate)) {
                            if (isCheckmate) {
                                cout << "\nCheckmate! " << (currentTurn == BALTA ? "White" : "Black") << " wins!" << endl;
                            } else {
                                cout << "\nStalemate! Game is a draw!" << endl;
                            }
                            cout << "Press any key to exit...";
                            _getch();
                            gameRunning = false;
                        } else {
                            currentTurn = nextTurn;
                        }
                    } else {
                        cout << "Invalid move. Press any key to continue...";
                        _getch();
                        lenta.printLenta();
                    }
                }
            } catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
                cout << "Press any key to continue..."; _getch(); lenta.printLenta();
            } catch (...) {
                cout << "Unknown error occurred. Press any key to continue..."; _getch(); lenta.printLenta();
            }
        }
        return 0;
    } catch (const exception& e) {
        cout << "Fatal error: " << e.what() << endl;
        cout << "Press any key to exit..."; _getch(); return 1;
    } catch (...) {
        cout << "Unknown fatal error occurred. Press any key to exit..."; _getch(); return 1;
    }
}
