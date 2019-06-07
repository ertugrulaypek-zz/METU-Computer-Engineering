`timescale 1ns / 1ps
module FUNCROM (input [3:0] romAddress, output reg[4:0] romData);

/*Write your code here*/

always@(romAddress)
		begin
			
			if(romAddress==4'b1110)
				begin
					romData=5'b11110;
				end
			else if(romAddress==4'b0000 | romAddress==4'b0001 | romAddress==4'b0010 | romAddress==4'b0111 | romAddress==4'b1001 | romAddress==4'b1010 | romAddress==4'b1101)
				begin
					romData[4:1]=romAddress;
					
					romData[0]=1'b0;
				end
			else if(romAddress==4'b0011 | romAddress==4'b0101 | romAddress==4'b0110 | romAddress==4'b1000 | romAddress==4'b1011 | romAddress==4'b1100 | romAddress==4'b1111)
				begin
					romData[4:1]=romAddress;
					
					romData[0]=1'b1;
				end
			else
				begin
					romData=5'b01010;
				end
		end
endmodule
																						
module FUNCRAM (input mode,input [3:0] ramAddress, input [4:0] dataIn,input op, input [1:0] arg,  input CLK, output reg [8:0] dataOut);
/*Write your code here*/

reg [8:0]a[15:0];
	integer store=0;
	integer i=0;
	
	initial begin
		a[0]=9'b000000000;
		a[1]=9'b000000000;
		a[2]=9'b000000000;
		a[3]=9'b000000000;
		a[4]=9'b000000000;
		a[5]=9'b000000000;
		a[6]=9'b000000000;
		a[7]=9'b000000000;
		a[8]=9'b000000000;
		a[9]=9'b000000000;
		a[10]=9'b000000000;
		a[11]=9'b000000000;
		a[12]=9'b000000000;
		a[13]=9'b000000000;
		a[14]=9'b000000000;
		a[15]=9'b000000000;
	end
					
		
	always@(posedge CLK)
begin
	if(mode==1)
	begin
		if(op==1)
		begin
			store = 0;
			if(arg==2'b00)
			begin
				for(i=1;i<=4;i=i+1)
				begin
					if(dataIn[i]==0) store=store+i*(2**(i-1));
					if(dataIn[i]==1) store=store-i*(2**(i-1));
				end
			end

			if(arg==2'b01)
			begin
				for(i=1;i<=4;i=i+1)
				begin
					if(dataIn[i]==0) store=store+i*(1**(i-1));
					if(dataIn[i]==1) store=store-i*(1**(i-1));
				end
			end

			if(arg==2'b10)
			begin
				for(i=1;i<=4;i=i+1)
				begin
					if(dataIn[i]==0) store=store+i*((-1)**(i-1));
					if(dataIn[i]==1) store=store-i*((-1)**(i-1));
				end
			end

			if(arg==2'b11)
			begin
				for(i=1;i<=4;i=i+1)
				begin
					if(dataIn[i]==0) store=store+i*((-2)**(i-1));
					if(dataIn[i]==1) store=store-i*((-2)**(i-1));
				end
			end

			if(store<0)
			begin
				dataOut[8]=1;
				dataOut[7:0]=-store;
				a[ramAddress]=dataOut;

			end
			
			if(store>=0)
			begin
				dataOut[8]=0;
				dataOut[7:0]=store;
				a[ramAddress]=dataOut;
			end
			store=0;
		end


		else
		begin
			store=0;
			if(arg==2'b00)
			begin
				for(i=0;i<=4;i=i+1)
				begin
					if(dataIn[i]==0) store=store+(2**i);
				    if(dataIn[i]==1) store=store-(2**i);			
				end
			end	

		   if(arg==2'b01)
			begin
				for(i=0;i<=4;i=i+1)
				begin
					if(dataIn[i]==0) store=store+(1**i);
				    if(dataIn[i]==1) store=store-(1**i);			
				end
			end	

			if(arg==2'b10)
			begin
				for(i=0;i<=4;i=i+1)
				begin
					if(dataIn[i]==0) store=store+((-1)**i);
				   if(dataIn[i]==1) store=store-((-1)**i);
											 
				end
			end	

			if(arg==2'b11)
			begin
				for(i=0;i<=4;i=i+1)
				begin
					if(dataIn[i]==0) store=store+((-2)**i);
				    if(dataIn[i]==1) store=store-((-2)**i);			
				end
			end	
			
			store=store%7;
			
			if(store<0) store=store+7;
			
			dataOut[8]=0;
			dataOut[7:0]=store;
			a[ramAddress]=dataOut;
			store=0;
		end
		
	end
	
end


always@(mode | ramAddress)	
begin
	if(mode==0)	 dataOut=a[ramAddress];		
end	
endmodule


module FUNCMEMORY(input mode, input [6:0] memInput, input CLK, output wire [8:0] result);
	/*Don't edit this module*/
	wire [4:0]  romData;

	FUNCROM RO(memInput[6:3], romData);
	FUNCRAM RA(mode, memInput[6:3], romData, memInput[2],memInput[1:0], CLK, result);

endmodule
