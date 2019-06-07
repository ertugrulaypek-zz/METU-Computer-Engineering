#include "common.h"


/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
char data[10];
char data_size;
char X,Y,Z,T;
char rotate[]="$R:";
char forward[]="$F:";
char stop[]="$S:";
char map[]="$M000:";
char left[]="$L:";
char end_command[]="$END:";

char state='I';



char curr_maze_status[4][16] = {
    {" XXXXXXXXXXXXXX "},{"XXXXXXXXXXXXXXXX"},{"XXXXXXXXXXXXXXXX"},{"XXXXXXXXXXXXXXXX"}
};

struct coord{
     char x;
     char y;
};
struct curr_coord{
    int x;
    int y;
};
struct curr_coord current_coord;
struct coord current_cell;
int curr_rotation;
int count_explored_cells;





/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/


void normalizeRotation(void){
    if(curr_rotation<0) curr_rotation+=360;
    if(curr_rotation>=360) curr_rotation-=360;
}


void rotateToRight(int given){ // rotates robot right according to given degree
    if(given<0) given+=360;
    if(given>=360) given-=360;
    
    while(1){
        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);
        
        normalizeRotation();
        transmitBuffer_push(rotate,3);
        WaitEvent(TRANSMIT_EVENT);
        ClearEvent(TRANSMIT_EVENT);
        
        WaitEvent(RECEIVE_EVENT);
        ClearEvent(RECEIVE_EVENT);
        data_size = receiveBuffer_pop(data);
         
            if(data[0]=='E'){
                curr_rotation-=data[1];
            }
            
        normalizeRotation();
        if(given==curr_rotation) break;
    }
    
}
void rotateToLeft(){ // rotates robot 90 degrees left
    normalizeRotation();
    while(1){
              
        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);
        
        transmitBuffer_push(left,3);
        WaitEvent(TRANSMIT_EVENT);
        ClearEvent(TRANSMIT_EVENT);
                                    
        WaitEvent(RECEIVE_EVENT);
        ClearEvent(RECEIVE_EVENT);
        data_size = receiveBuffer_pop(data);
                                    
        curr_rotation+=data[1];

        if(curr_rotation == 0 | curr_rotation==360 | curr_rotation==90 | curr_rotation==180 | curr_rotation==270){
            normalizeRotation();
            break;
        }
    }
}
int calculateTargetXcoord(void){ // returns x coord of robot's front cell coordinate
    if(curr_rotation==0){
        return current_coord.x-50;
    }
    else if(curr_rotation==90){
        return current_coord.x;
    }
    else if(curr_rotation==180){
        return current_coord.x+50;
    }
    else if(curr_rotation==270){
        return current_coord.x;
    }
}

int calculateTargetYcoord(void){ // returns y coord of robot's front cell coordinate
    if(curr_rotation==0){
        return current_coord.y;
    }
    else if(curr_rotation==90){
        return current_coord.y-50;
    }
    else if(curr_rotation==180){
        return current_coord.y;
    }
    else if(curr_rotation==270){
        return current_coord.y+50;
    }
}

void moveToCoord(int x, int y){ //move to given coordinate, assumes coordinate to move is in front of robot
    while(1){

        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);
        
        transmitBuffer_push(forward,3);
        
        WaitEvent(RECEIVE_EVENT);
        ClearEvent(RECEIVE_EVENT);
        data_size = receiveBuffer_pop(data);
            
            if(data[0]=='E'){
                if(curr_rotation==180){
                    current_coord.x+=data[1];
                }
                else if(curr_rotation==0){
                    current_coord.x-=data[1];
                }
                else if(curr_rotation==90){
                    current_coord.y-=data[1];
                }
                else if(curr_rotation==270){
                    current_coord.y+=data[1];
                }   
            }
        if( (x==current_coord.x) && (y==current_coord.y) ) break;
    }
}

void updateCurrCell(void){ //update cell coordinate according to robot's coordinate
    
    current_cell.x= current_coord.x/50;
    current_cell.y= current_coord.y/50;
    
}

//Communication functions between lcd and task1:
char* getFirstRow(){
    return curr_maze_status[0];
}
char* getSecondRow(){
    return curr_maze_status[1];
}
char* getThirdRow(){
    return curr_maze_status[2];
}
char* getFourthRow(){
    return curr_maze_status[3];
}


void updateMap(char X, char Y, char Z, char T){ // Updates map according to sensor values:
    updateCurrCell();
   
    if(curr_rotation==0){
        if(((int)current_cell.x)-1>=0){
            if(curr_maze_status[current_cell.x-1][current_cell.y] == 'X'){
                count_explored_cells++;
            }
            curr_maze_status[current_cell.x-1][current_cell.y]= (X==0 ? ' ' : 0xFF);
            map[2]=(current_cell.x-1);
            map[3]=(current_cell.y);
            map[4]=X;  
            transmitBuffer_push(map,6);         
            WaitEvent(TRANSMIT_EVENT);
            ClearEvent(TRANSMIT_EVENT);
        }
        
        if(current_cell.y+1<=15){
        if(curr_maze_status[current_cell.x][current_cell.y+1] == 'X'){
                count_explored_cells++;
            }
        curr_maze_status[current_cell.x][current_cell.y+1]= (Y==0 ? ' ' : 0xFF);
        map[2]=(current_cell.x);
        map[3]=(current_cell.y+1);
        map[4]=Y;
        transmitBuffer_push(map,6);
        WaitEvent(TRANSMIT_EVENT);
        ClearEvent(TRANSMIT_EVENT);
        }
        
        if(current_cell.x+1<=3){
            if(curr_maze_status[current_cell.x+1][current_cell.y] == 'X'){
                count_explored_cells++;
            }
        curr_maze_status[current_cell.x+1][current_cell.y]= (Z==0 ? ' ' : 0xFF);
        map[2]=(current_cell.x+1);
        map[3]=(current_cell.y);
        map[4]=Z;
        transmitBuffer_push(map,6);
        WaitEvent(TRANSMIT_EVENT);
        ClearEvent(TRANSMIT_EVENT);
        }
        
        if(((int)current_cell.y)-1>=0){
            if(curr_maze_status[current_cell.x][current_cell.y-1] == 'X'){
                count_explored_cells++;
            }
            
        curr_maze_status[current_cell.x][current_cell.y-1]= (T==0 ? ' ' : 0xFF);
        map[2]=(current_cell.x);
        map[3]=(current_cell.y-1);
        map[4]=T;
        transmitBuffer_push(map,6);
        WaitEvent(TRANSMIT_EVENT);
        ClearEvent(TRANSMIT_EVENT);
        }
        
    }
    else if(curr_rotation==90){
        if(((int)current_cell.y)-1>=0){
            if(curr_maze_status[current_cell.x][current_cell.y-1] == 'X'){
                count_explored_cells++;
            }
        curr_maze_status[current_cell.x][current_cell.y-1]= (X==0 ? ' ' : 0xFF);
        map[2]=(current_cell.x);
        map[3]=(current_cell.y-1);
        map[4]=X;                
        transmitBuffer_push(map,6);
        WaitEvent(TRANSMIT_EVENT);
        ClearEvent(TRANSMIT_EVENT);
        }
            
        if(((int)current_cell.x)-1>=0){
            if(curr_maze_status[current_cell.x-1][current_cell.y] == 'X'){
                count_explored_cells++;
            }
        curr_maze_status[current_cell.x-1][current_cell.y]= (Y==0 ? ' ' : 0xFF);
        map[2]=(current_cell.x-1);
        map[3]=(current_cell.y);
        map[4]=Y;
        transmitBuffer_push(map,6);
        WaitEvent(TRANSMIT_EVENT);
        ClearEvent(TRANSMIT_EVENT);
        }
        
        if(current_cell.y+1<=15){
            if(curr_maze_status[current_cell.x][current_cell.y+1] == 'X'){
                count_explored_cells++;
            }
        curr_maze_status[current_cell.x][current_cell.y+1]= (Z==0 ? ' ' : 0xFF);
        map[2]=(current_cell.x);
        map[3]=(current_cell.y+1);
        map[4]=Z;
        transmitBuffer_push(map,6);
        WaitEvent(TRANSMIT_EVENT);
        ClearEvent(TRANSMIT_EVENT);
        }
        
        
        if(current_cell.x+1<=3){
            if(curr_maze_status[current_cell.x+1][current_cell.y] == 'X'){
                count_explored_cells++;
            }
        curr_maze_status[current_cell.x+1][current_cell.y]= (T==0 ? ' ' : 0xFF);
        map[2]=(current_cell.x+1);
        map[3]=(current_cell.y);
        map[4]=T;
        transmitBuffer_push(map,6);
        WaitEvent(TRANSMIT_EVENT);
        ClearEvent(TRANSMIT_EVENT);
        }
    }
    
    else if(curr_rotation==180){
        
        if(current_cell.x+1<=3){
            if(curr_maze_status[current_cell.x+1][current_cell.y] == 'X'){
                count_explored_cells++;
            }
        curr_maze_status[current_cell.x+1][current_cell.y]= (X==0 ? ' ' : 0xFF);
        map[2]=(current_cell.x+1);
        map[3]=(current_cell.y);
        map[4]=X;    
        transmitBuffer_push(map,6);
        WaitEvent(TRANSMIT_EVENT);
        ClearEvent(TRANSMIT_EVENT);
        }
        
        
        if(((int)current_cell.y)-1>=0){
            if(curr_maze_status[current_cell.x][current_cell.y-1] == 'X'){
                count_explored_cells++;
            }
        curr_maze_status[current_cell.x][current_cell.y-1]= (Y==0 ? ' ' : 0xFF);
        map[2]=(current_cell.x);
        map[3]=(current_cell.y-1);
        map[4]=Y;
        transmitBuffer_push(map,6);
        WaitEvent(TRANSMIT_EVENT);
        ClearEvent(TRANSMIT_EVENT);
        }
        
        if(((int)current_cell.x)-1>=0){
            if(curr_maze_status[current_cell.x-1][current_cell.y] == 'X'){
                count_explored_cells++;
            }
        curr_maze_status[current_cell.x-1][current_cell.y]= (Z==0 ? ' ' : 0xFF);
        map[2]=(current_cell.x-1);
        map[3]=(current_cell.y);
        map[4]=Z;
        transmitBuffer_push(map,6);
        WaitEvent(TRANSMIT_EVENT);
        ClearEvent(TRANSMIT_EVENT);
        }
        
        if(current_cell.y+1<=15){
            if(curr_maze_status[current_cell.x][current_cell.y+1] == 'X'){
                count_explored_cells++;
            }
        curr_maze_status[current_cell.x][current_cell.y+1]= (T==0 ? ' ' : 0xFF);
        map[2]=(current_cell.x);
        map[3]=(current_cell.y+1);
        map[4]=T;
        transmitBuffer_push(map,6);
        WaitEvent(TRANSMIT_EVENT);
        ClearEvent(TRANSMIT_EVENT);
        }
    }
    else if(curr_rotation==270){
        if(current_cell.y+1<=15){
            if(curr_maze_status[current_cell.x][current_cell.y+1] == 'X'){
                count_explored_cells++;
            }
        curr_maze_status[current_cell.x][current_cell.y+1]= (X==0 ? ' ' : 0xFF);
        map[2]=(current_cell.x);
        map[3]=(current_cell.y+1);
        map[4]=X; 
        transmitBuffer_push(map,6);
        WaitEvent(TRANSMIT_EVENT);
        ClearEvent(TRANSMIT_EVENT);
        }
        
        
        if(current_cell.x+1<=3){
            if(curr_maze_status[current_cell.x+1][current_cell.y] == 'X'){
                count_explored_cells++;
            }
        curr_maze_status[current_cell.x+1][current_cell.y]= (Y==0 ? ' ' : 0xFF);
        map[2]=(current_cell.x+1);
        map[3]=(current_cell.y);
        map[4]=Y;
        transmitBuffer_push(map,6);
        WaitEvent(TRANSMIT_EVENT);
        ClearEvent(TRANSMIT_EVENT);
        }
        
        if(((int)current_cell.y)-1>=0){
            if(curr_maze_status[current_cell.x][current_cell.y-1] == 'X'){
                count_explored_cells++;
            }
        curr_maze_status[current_cell.x][current_cell.y-1]= (Z==0 ? ' ' : 0xFF);
        map[2]=(current_cell.x);
        map[3]=(current_cell.y-1);
        map[4]=Z;
        transmitBuffer_push(map,6);
        WaitEvent(TRANSMIT_EVENT);
        ClearEvent(TRANSMIT_EVENT);
        }
        
        if(((int)current_cell.x)-1>=0){
            if(curr_maze_status[current_cell.x-1][current_cell.y] == 'X'){
                count_explored_cells++;
            }
        curr_maze_status[current_cell.x-1][current_cell.y]= (T==0 ? ' ' : 0xFF);
        map[2]=(current_cell.x-1);
        map[3]=(current_cell.y);
        map[4]=T;
        transmitBuffer_push(map,6);
        WaitEvent(TRANSMIT_EVENT);
        ClearEvent(TRANSMIT_EVENT);
        }
    }
    
}


/**********************************************************************
 * ------------------------------ TASK1 -------------------------------
 *
 * Basic echo function test. Type $hello: in cutecom and receive hello
 *
 **********************************************************************/
TASK(TASK1) 
{
    
    
    SetRelAlarm(ALARM_TSK1, 40, 40);
    
        
    
    PIE1bits.RCIE= 1;
    curr_rotation=180;
    
    current_coord.x=25;
    current_coord.y=25;
    
    current_cell.x=0;
    current_cell.y=0;
    
    count_explored_cells=2;
    
    
    
    
    while(1){
        
        // IDLE STATE:
        if(state=='I') {
            
            WaitEvent(ALARM_EVENT);
            ClearEvent(ALARM_EVENT);
            
            SetEvent(LCD_ID,LCD_EVENT);
            
            WaitEvent(RECEIVE_EVENT);
            ClearEvent(RECEIVE_EVENT);
            data_size = receiveBuffer_pop(data);

            if(data[0]=='G'){                                           
                state='A';                
            }        
        }  
    
    
        // ACTIVE STATE:
        else if(state=='A'){
            
            if(getTickCount()%10==0){ // every 400ms wake LCD task up
                SetEvent(LCD_ID,LCD_EVENT);
            }
            
            WaitEvent(ALARM_EVENT);
            ClearEvent(ALARM_EVENT);
            
            
            

            transmitBuffer_push(stop,3);
            WaitEvent(TRANSMIT_EVENT);
            ClearEvent(TRANSMIT_EVENT);
                    
            WaitEvent(RECEIVE_EVENT);
            ClearEvent(RECEIVE_EVENT);
            data_size = receiveBuffer_pop(data);
                    
               
            if(data[0]=='D'){
                updateMap(data[1],data[2],data[3],data[4]);
                if(count_explored_cells==64){
                    state='T';                                         
                }       
                if(data[2]==0){ // if robot's right is empty, turn there and move:
                    
                    rotateToRight(curr_rotation-90);
                    moveToCoord(calculateTargetXcoord(),calculateTargetYcoord());
                }
                else if(data[1]==0){ // if there is no obstacle in front of robot, then move:
                    moveToCoord(calculateTargetXcoord(),calculateTargetYcoord());
                }

                else{ // turn left:
                    rotateToLeft();                                                                                                                                                        
                }
            }
            
        }
        
        
        // TARGET STATE (ALL CELLS EXPLORED) FOR BONUS PART:        
        else if(state=='T'){
            WaitEvent(ALARM_EVENT);
            ClearEvent(ALARM_EVENT);
            
            updateCurrCell();
            if(current_cell.x==0 && current_cell.y==15 ){
                transmitBuffer_push(end_command,5);
                WaitEvent(TRANSMIT_EVENT);
                ClearEvent(TRANSMIT_EVENT);
                TerminateTask();
            }
            
            transmitBuffer_push(stop,3);
            WaitEvent(TRANSMIT_EVENT);
            ClearEvent(TRANSMIT_EVENT);
                    
            WaitEvent(RECEIVE_EVENT);
            ClearEvent(RECEIVE_EVENT);
            data_size = receiveBuffer_pop(data);
                    
               
            // INVERSE OF ACTIVE STATE:
            if(data[0]=='D'){
                
                updateCurrCell();
                
                if(data[4]==0){ // if robot's left is empty, turn there and move:
                    rotateToLeft();
                    
                    moveToCoord(calculateTargetXcoord(),calculateTargetYcoord());
                }
                else if(data[1]==0){ // if there is no obstacle in front of robot, then move:
                    moveToCoord(calculateTargetXcoord(),calculateTargetYcoord());
                }

                else{ // turn right:
                    rotateToRight(curr_rotation-90);                                                                                                                                                                          
                }
            }
                                    
        }
    }
    
    TerminateTask();
}

/* End of File : tsk_task1.c */