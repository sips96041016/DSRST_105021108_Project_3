class Student{
    public:
        int remain(int i,int j) { return _Max[i][j] - _Record[i][j]; }
        bool edge_white(int i,int j,char dir,int Record[5][6], int Max[5][6], Color color[5][6]){
            if(dir=='i') return  ( j==0 || Record[i][j-1]<=1 ) && ( j==5 || Record[i][j+1]<=1 ) &&
                                 ( i==4 || color[i+1][j] == White ) && ( i==0 || color[i-1][j] == White );
            else return  ( i==0 || Record[i-1][j]<=1 ) && ( i==4 || Record[i+1][j]<=1 ) &&
                         ( j==5 || color[i][j+1] == White ) && ( j==0 || color[i][j-1] == White );
        }
        bool check_conner(int i,int h,int j,int k,int Record[5][6], int Max[5][6], Color color[5][6]){
            Color inputColor = color[i][j], rivalColor = (inputColor == Red)? Blue : Red;
            return ( i+h>=0 && i+h<=4 && j+k>=0 && j+k<=6 ) && color[i+h][j+k] == rivalColor &&
                   ( (color[i+h][j] == inputColor && remain(i,j) <= remain(i+h,j) + remain(i+h,j+k))||
                     (color[i][j+k] == inputColor && remain(i,j) <= remain(i,j+k) + remain(i+h,j+k)) ) ;
        }
        bool near_all_white(int i,int j,int Record[5][6], int Max[5][6], Color color[5][6]){
            return  (i==4||color[i+1][j] == White) && (i==0||color[i-1][j] == White) &&
                    (j==5||color[i][j+1] == White) && (j==0||color[i][j-1] == White);
        }
        bool be_force(int i,int j,int Record[5][6], int Max[5][6], Color color[5][6], bool strict){
            Color rivalColor = (color[i][j] == Red)? Blue : Red;
            if(strict) return (  (i<4)&&(color[i+1][j] == rivalColor && remain(i+1,j) < remain(i,j))  )||
                              (  (i>0)&&(color[i-1][j] == rivalColor && remain(i-1,j) < remain(i,j))  )||
                              (  (j<5)&&(color[i][j+1] == rivalColor && remain(i,j+1) < remain(i,j))  )||
                              (  (j>0)&&(color[i][j-1] == rivalColor && remain(i,j-1) < remain(i,j))  ) ;
            else return (  (i<4)&&(color[i+1][j] == rivalColor && remain(i+1,j) <= remain(i,j))  )||
                        (  (i>0)&&(color[i-1][j] == rivalColor && remain(i-1,j) <= remain(i,j))  )||
                        (  (j<5)&&(color[i][j+1] == rivalColor && remain(i,j+1) <= remain(i,j))  )||
                        (  (j>0)&&(color[i][j-1] == rivalColor && remain(i,j-1) <= remain(i,j))  ) ;
        }
        void makeMove(int Record[5][6], int Max[5][6], Color color[5][6], Color inputColor){
            int i,j;
            Color rivalColor = (inputColor == Red)? Blue : Red;
            for(i=0;i<5;i++) for(j=0;j<6;j++) { _Max[i][j] = Max[i][j]; _Record[i][j] = Record[i][j]; }
            //先下角落
            if(color[0][0] == White) { x=0; y=0; return; }
            else if(color[4][5] == White) { x=4; y=5; return; }
            else if(color[4][0] == White) { x=4; y=0; return; }
            else if(color[0][5] == White) { x=0; y=5; return; }
            //比對方先爆
            for(i=0;i<5;i++) for(j=0;j<6;j++)
                if(color[i][j] == inputColor && remain(i,j) == 1 && be_force(i,j,Record,Max,color,false) ) { x=i; y=j; return; }

            //比對方接近爆(自己被威脅)
            for(i=0;i<5;i++) for(j=0;j<6;j++){
                //若已追不上，直接算了；追得上的話，若對方沒有被我威脅，蓋上去
                if( color[i][j] == inputColor && remain(i,j) > 1 && !be_force(i,j,Record,Max,color,true) &&
                    ( ( i<4 && color[i+1][j]==rivalColor && remain(i+1,j)==remain(i,j) && !be_force(i+1,j,Record,Max,color,true) )||
                      ( i>0 && color[i-1][j]==rivalColor && remain(i-1,j)==remain(i,j) && !be_force(i-1,j,Record,Max,color,true) )||
                      ( j<5 && color[i][j+1]==rivalColor && remain(i,j+1)==remain(i,j) && !be_force(i,j+1,Record,Max,color,true) )||
                      ( j>0 && color[i][j-1]==rivalColor && remain(i,j-1)==remain(i,j) && !be_force(i,j-1,Record,Max,color,true) ) ) ){
                    x=i; y=j; return;
                }
            }
            //edge 空白 間隔下
            for(j=1;j<5;j++){
                if(color[0][j] == White && !be_force(0,j,Record,Max,color,true) ) { x=0; y=j; return; }
                if(color[4][j] == White && !be_force(4,j,Record,Max,color,true) ) { x=4; y=j; return; }
            }
            for(i=1;i<4;i++){
                if(color[i][0] == White && !be_force(i,0,Record,Max,color,true) ) { x=i; y=0; return; }
                if(color[i][5] == White && !be_force(i,5,Record,Max,color,true) ) { x=i; y=5; return; }
            }
            //center 空白 間隔下
            for(i=1;i<4;i++) for(j=1;j<5;j++) if( color[i][j]==White && near_all_white(i,j,Record,Max,color) ) { x=i; y=j; return; }
            //edge 自己 間隔下
            for(j=0;j<6;j++){
                if( color[0][j] == inputColor && remain(0,j)>1 && edge_white(0,j,'j',Record,Max,color) ) { x=0; y=j; return; }
                if( color[4][j] == inputColor && remain(4,j)>1 && edge_white(4,j,'j',Record,Max,color) ) { x=4; y=j; return; }
            }
            for(i=0;i<5;i++){
                if( color[i][0] == inputColor && remain(i,0)>1 && edge_white(i,0,'i',Record,Max,color) ) { x=i; y=0; return; }
                if( color[i][5] == inputColor && remain(i,5)>1 && edge_white(i,5,'i',Record,Max,color) ) { x=i; y=5; return; }
            }
            //center 自己 間隔下
            for(i=1;i<4;i++) for(j=1;j<5;j++)
                if(color[i][j] == inputColor && remain(i,j)>1 && near_all_white(i,j,Record,Max,color)) { x=i; y=j; return; }
            //若有白的先下
            for(i=0;i<5;i++) for(j=0;j<6;j++) if(color[i][j] == White) { x=i; y=j; return; }
            //差兩格
            for(i=0;i<5;i++) for(j=0;j<6;j++) if(color[i][j] == inputColor && remain(i,j)>1){
                if((color[i+2][j] == rivalColor && color[i+1][j] == inputColor && remain(i,j) <= remain(i+1,j) + remain(i+2,j) )||
                   (color[i-2][j] == rivalColor && color[i-1][j] == inputColor && remain(i,j) <= remain(i-1,j) + remain(i-2,j) )||
                   (color[i][j+2] == rivalColor && color[i][j+1] == inputColor && remain(i,j) <= remain(i,j+1) + remain(i,j+2) )||
                   (color[i][j-2] == rivalColor && color[i][j-1] == inputColor && remain(i,j) <= remain(i,j-1) + remain(i,j-2) )||
                   check_conner(i,1,j,1,Record,Max,color)  || check_conner(i,1,j,-1,Record,Max,color) ||
                   check_conner(i,-1,j,1,Record,Max,color) || check_conner(i,-1,j,-1,Record,Max,color) )
                   { x=i; y=j; return; }
            }
            //還沒爆的先
            for(i=0;i<5;i++) for(j=0;j<6;j++) if(color[i][j] == inputColor && remain(i,j)>1 ) { x=i; y=j; return; }
            for(i=0;i<5;i++) for(j=0;j<6;j++) if(color[i][j] == inputColor) { x=i; y=j; return; }
        }
        int getX(){
            return x;
        }
        int getY(){
            return y;
        }
    private:
        int x;
        int y;
        int _Record[5][6];
        int _Max[5][6];
    };
