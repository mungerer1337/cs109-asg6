//Michael U.
//Hex game
//Persson vs Monte Carlo AI. 
//If you are reading this and need a hint go to line 175 ;)

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <list>
using namespace std;

inline int rand_hex(){ return rand() % 11;}  //inline function to return a random hex coordinate

//node class to hold hex information
class node{
private:
   int row;      //row coordinate
   int col;      //column coordinate
   int color;    //color for path finding
   int owner;    //owner of node
public:
   node();                 //default constructor
   node(int r,int c);      //constructor with coordinates
   int get_row();
   int get_col();
   int get_color();
   int get_owner();
   void change_owner(int player);
   void change_color(int c);
};

node::node(){
   row = 0;
   col = 0;
   color = 0;
   owner = 0;
}

node::node(int r, int c){
   row = r;
   col = c;
   color = 0;
   owner = 0;
}

int node::get_row(){
   return row;
}

int node::get_col(){
   return col;
}

int node::get_color(){
   return color;
}

int node::get_owner(){
   return owner;
}

void node::change_owner(int player){
   owner = player;
}

void node::change_color(int c){
   color = c;
}


//hexboard class containing a 2D vector of hex nodes
class hexboard{
private:
   int size;                              //size of board
   vector < vector <node> > matrix;       //matrix of the board
   
public:
   hexboard(int s);      
   void print_board();
   void fill_board();
   void find_winner();
   friend ostream& operator<<(ostream& out, hexboard h);
   void printhex();
};

//constructor for hexboard takes in size as parameter
hexboard::hexboard(int s){
   size = s;
   matrix.resize(size);               //resize vector containing node vector
   for(int i = 0; i < size; ++i)      //resize node vectors
      matrix[i].resize(size);

   for(int i = 0; i < size; ++i){
      for(int j = 0; j < size; ++j)   //fill in node vectors with nodes containing coordinates
         matrix[i][j] = node(i,j);
   }

}

//function to fill in the hexboard randomly
void hexboard::fill_board(){             
   int row = rand_hex();
   int col = rand_hex();
   for(int i = 0; i < 61; ++i){
      while(matrix[row][col].get_owner() != 0){    //get random coordinates until an empty one is found
         row = rand_hex();
         col = rand_hex();
      }
      matrix[row][col].change_owner(1);            //change owner to player 1
      if(i == 60) continue;
      while(matrix[row][col].get_owner() != 0){    //get random coordinates until an empty one is found
         row = rand_hex();
         col = rand_hex();
      }
      matrix[row][col].change_owner(2);            //change owner to player 2
   }
}

//perform BFS to find a path for player 1. If not found, player 2 won
void hexboard::find_winner(){
   int row = 0;
   list <node*> queue;
   for(int col = 0; col < size; ++col){       //begin at 0,0 coordinate
      if(matrix[row][col].get_owner() == 1){
         if(matrix[row][col].get_color() == 1 || matrix[row][col].get_color() == 2) continue; 
         queue.push_back(&matrix[row][col]);
         matrix[row][col].change_color(1);
         while(queue.size() != 0){           //start BFS loop. Outer if statements check for edge cases. If node is owned by player 1 and unvisited, add to queue.
            node* curr = queue.front();   
            if(curr->get_row() != 0){
               if(matrix[curr->get_row()-1][curr->get_col()].get_owner() == 1 && matrix[curr->get_row()-1][curr->get_col()].get_color() == 0){
                  queue.push_back(&matrix[curr->get_row()-1][curr->get_col()]);
                  matrix[curr->get_row()-1][curr->get_col()].change_color(1);
               }
            }
            if(curr->get_row() != 0 && curr->get_col() != size-1){
               if(matrix[curr->get_row()-1][curr->get_col()+1].get_owner() == 1 && matrix[curr->get_row()-1][curr->get_col()+1].get_color() == 0){
                  queue.push_back(&matrix[curr->get_row()-1][curr->get_col()+1]);
                  matrix[curr->get_row()-1][curr->get_col()+1].change_color(1);
               } 
            }
            if(curr->get_col() != 0){
               if(matrix[curr->get_row()][curr->get_col()-1].get_owner() == 1 && matrix[curr->get_row()][curr->get_col()-1].get_color() == 0){
                  queue.push_back(&matrix[curr->get_row()][curr->get_col()-1]);
                  matrix[curr->get_row()][curr->get_col()-1].change_color(1);
               }
            }
            if(curr->get_col() != size-1){
               if(matrix[curr->get_row()][curr->get_col()+1].get_owner() == 1 && matrix[curr->get_row()][curr->get_col()+1].get_color() == 0){
                  queue.push_back(&matrix[curr->get_row()][curr->get_col()+1]);
                  matrix[curr->get_row()][curr->get_col()+1].change_color(1);
               }
            }
            if(curr->get_row() != size-1){
               if(matrix[curr->get_row()+1][curr->get_col()].get_owner() == 1 && matrix[curr->get_row()+1][curr->get_col()].get_color() == 0){               
                  queue.push_back(&matrix[curr->get_row()+1][curr->get_col()]);
                  matrix[curr->get_row()+1][curr->get_col()].change_color(1);
               }
            }
            if(curr->get_row() != size-1 && curr->get_col() != 0){
               if(matrix[curr->get_row()+1][curr->get_col()-1].get_owner() == 1 && matrix[curr->get_row()+1][curr->get_col()-1].get_color() == 0){
                  queue.push_back(&matrix[curr->get_row()+1][curr->get_col()-1]);
                  matrix[curr->get_row()+1][curr->get_col()-1].change_color(1);
               }
            }

         queue.pop_front();         //pop off queue and change node to visited
         curr->change_color(2);

         }
      }
   }
   for(int i = 0; i < size; ++i){                //check opposite side of board for visited node. If found, player 1 won. Else player 2 won
      if(matrix[size-1][i].get_color() == 2){
         cout << endl << endl << "PLAYER 1 WINS! Thanks for playing :) my number is (510) 406-2115" << endl << endl << endl;
         return;
      }
   }
   cout << endl << endl << "PLAYER 2 WINS! Thanks for playing :) my number is (510) 406-2115" << endl << endl << endl;


}

//overload << operator to print out hexboards
ostream& operator<<(ostream& out, hexboard h){
   cout << "      P  L  A  Y  E  R  1  =  #" << endl;   //player 1 header
   cout << "  ";
   for(int i = 0; i < h.size; ++i){
      cout << "/ \\ ";
   }
   cout << endl;
   int q = 0;
   for(int i = 0; i < h.size; ++i){
      ++q;
      for(int z = 0 ; z < q; ++z){
         cout<<" ";
      }

      for(int j = 0; j < h.size; ++j){
         if(h.matrix[i][j].get_owner() == 1)
            cout<<"| # ";
         else  cout<<"| . ";
      }
      cout << "|"; 
      ++q;   
      cout << endl;
      for(int z = 0; z < q; ++z)
         cout<<" ";
      for(int j = 0; j < h.size; ++j){
         if(i + 1 == h.size)
            cout << "\\ / ";
         else if(j + 1 == h.size)
            cout << "\\ / \\";
         else cout << "\\ / ";
      }
      if(i == 0) cout << "     P";         //player 2 header
      if(i == 1) cout << "     L";
      if(i == 2) cout << "     A";
      if(i == 3) cout << "     Y";
      if(i == 4) cout << "     E";
      if(i == 5) cout << "     R";
      if(i == 6) cout << "     2";
      if(i == 7) cout << "     =";
      if(i == 8) cout << "     .";
      cout << endl;
   }
   cout << endl;
   return out;
}





int main(){
   srand(time(0));                 //seed random generator
   hexboard test1 = hexboard(11);  //create a 11x11 hexboard
   test1.fill_board();             //fill the board randomly
   test1.find_winner();            //find the winner
   cout << test1;                  //print the hexboard
   return 0;
}








