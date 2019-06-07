`timescale 1ns / 1ps

module Elevator(input CLK,
					input [1:0] mode,
					input [3:0] request,
					output reg[3:0] currentFloor1,
					output reg[3:0] currentFloor2,
					output reg doorOpen1, 
					output reg doorOpen2, 
					output reg [3:0] listingLeds,
					output reg listBusy,
					output reg state);

//Write your code below
reg [3:0]taskPool[7:0];
integer i=0,k=0;
integer listingCursor=0;
integer place=0;
integer sameTask=0,targetFloor1,targetFloor2;
initial begin
	currentFloor1[3:0] = 1;
	currentFloor2[3:0] = 1;
	doorOpen1=1;
	doorOpen2=1;
	listingLeds=4'b0000;
	listBusy = 0;
	state = 0;
	taskPool[0]=4'b0000;
	taskPool[1]=4'b0000;
	taskPool[2]=4'b0000;
	taskPool[3]=4'b0000;
	taskPool[4]=4'b0000;
	taskPool[5]=4'b0000;
	taskPool[6]=4'b0000;
	taskPool[7]=4'b0000;

end
always@(posedge CLK)
begin


if(state==0)
	begin
		
		if(mode==2'b00)
			begin
				listingLeds=4'b0000;
				sameTask=0;
				place=-1;
				for(i=0;i<8;i=i+1)
					begin
						if(taskPool[i]==request) sameTask=1;
					end
				if(sameTask==0)
					begin
						for(i=7;i>=0;i=i-1)
							begin
								if(taskPool[i]==4'b0000) place=i;
							end
						if(place!=-1) taskPool[place]=request;
					end
			end
			
			
		else if(mode==2'b01)
			begin
				listBusy=1;
				if(listingCursor==8 | taskPool[listingCursor]==4'b0000)
					begin
						listBusy=0;
						listingCursor=0;
						listingLeds=4'b0000;
					end
				else
					begin
						listingLeds=taskPool[listingCursor];
						listingCursor=listingCursor+1;
					end
			end
			
		else if(mode==2'b10)
			begin			
				listingLeds=4'b0000;
				for(i=0;i<8;i=i+1)
					begin
						if(taskPool[i]==request) 
							begin								
								taskPool[i]=4'b0000;							
								for(k=i;k<7;k=k+1)
									begin
										taskPool[k]=taskPool[k+1];
									end
								taskPool[7]=4'b0000;
							end
					end				
			end
		
		else if(mode==2'b11)
			begin
				listingLeds=4'b0000;
				state=1;
				if(taskPool[0]==4'b0000) state=0;
				else
					begin
						targetFloor1=taskPool[0];
						for(i=0;i<7;i=i+1)
							begin					 																											
								taskPool[i]=taskPool[i+1];															
							end
						taskPool[7]=4'b0000;
						targetFloor2=taskPool[0];
						for(i=0;i<7;i=i+1)
							begin					 																											
								taskPool[i]=taskPool[i+1];															
							end
						taskPool[7]=4'b0000;
					end
			end
				
	end //end of state==0 statement
	
if(state==1)
	begin
		doorOpen1=0;
		doorOpen2=0;
		if(targetFloor1==4'b0000)
			begin
				doorOpen1=1;
				targetFloor1=taskPool[0];
				for(i=0;i<7;i=i+1)
					begin					 																											
						taskPool[i]=taskPool[i+1];															
					end
				taskPool[7]=4'b0000;				
			end
		else
			begin
			   doorOpen1=0;
				if(currentFloor1>targetFloor1)
					begin
						currentFloor1=currentFloor1-1;
						if(currentFloor1==targetFloor1) 
							begin
								doorOpen1=1;
								targetFloor1=taskPool[0];
								for(i=0;i<7;i=i+1)
									begin					 																											
										taskPool[i]=taskPool[i+1];															
									end
								taskPool[7]=4'b0000;
							end
					end
				else if(currentFloor1==targetFloor1)
					begin
						doorOpen1=1;
						targetFloor1=taskPool[0];
							for(i=0;i<7;i=i+1)
								begin					 																											
									taskPool[i]=taskPool[i+1];															
								end
						taskPool[7]=4'b0000;
					end
				else
					begin
						currentFloor1=currentFloor1+1;
						if(currentFloor1==targetFloor1) 
							begin
								doorOpen1=1;
								targetFloor1=taskPool[0];
								for(i=0;i<7;i=i+1)
									begin					 																											
										taskPool[i]=taskPool[i+1];															
									end
								taskPool[7]=4'b0000;		
							end
					end					
			end
			
			
			
		if(targetFloor2==4'b0000)
			begin
				doorOpen2=1;
				targetFloor2=taskPool[0];
				for(i=0;i<7;i=i+1)
					begin					 																											
						taskPool[i]=taskPool[i+1];															
					end
				taskPool[7]=4'b0000;
			end
		else
			begin
				doorOpen2=0;
				if(currentFloor2>targetFloor2)
					begin
						currentFloor2=currentFloor2-1;
						if(currentFloor2==targetFloor2) 
							begin
								doorOpen2=1;
								targetFloor2=taskPool[0];
								for(i=0;i<7;i=i+1)
									begin					 																											
										taskPool[i]=taskPool[i+1];															
									end
								taskPool[7]=4'b0000;
							end
					end
				else if(currentFloor2==targetFloor2)
					begin
						doorOpen2=1;
						targetFloor2=taskPool[0];
							for(i=0;i<7;i=i+1)
								begin					 																											
									taskPool[i]=taskPool[i+1];															
								end
						taskPool[7]=4'b0000;
					end
				else
					begin
						currentFloor2=currentFloor2+1;
						if(currentFloor2==targetFloor2) 
							begin
								doorOpen2=1;
								targetFloor2=taskPool[0];
								for(i=0;i<7;i=i+1)
									begin					 																											
										taskPool[i]=taskPool[i+1];															
									end
								taskPool[7]=4'b0000;
							end
					end
			end
		
		
		if(targetFloor1==4'b0000 & targetFloor2==4'b0000 ) 
			begin
				state=0;
				doorOpen1=1;
				doorOpen2=1;
			end
			
			
	end




end //end of CLK
endmodule