`timescale 1ns / 1ps 
module lab3_2(
			input[4:0] word,
			input CLK, 
			input selection,
			input mode,
			output reg [7:0] hipsterians1,
			output reg [7:0] hipsterians0,
			output reg [7:0] nerdians1,
			output reg [7:0] nerdians0,
			output reg warning
    );

	initial begin
		hipsterians0=0;
		nerdians0=0;
		hipsterians1=0;
		nerdians1=0;
		warning=0;
	end
   //Modify the lines below to implement your design .
	always@(posedge CLK)
	begin
		if(selection==0)
			begin
				if((word[0]==0&word[1]==0) | (word[1]==0&word[2]==0) | (word[2]==0&word[3]==0) | (word[3]==0&word[4]==0))
					begin
						if(mode==1)
							begin
								if(hipsterians1==2)
									begin
										hipsterians0=0;
										hipsterians1=0;
									end							
								else if (hipsterians0==9)
									begin
										hipsterians0=0;
										hipsterians1=hipsterians1+1;
									end
								else hipsterians0=hipsterians0+1;
							end
						else
							begin
								if(hipsterians0==0 & hipsterians1==0) hipsterians0=0;
								else if(hipsterians0==0)
									begin
										hipsterians1=hipsterians1-1;
										hipsterians0=9;
									end
								else hipsterians0=hipsterians0-1;
							end
					end
				else 	warning=1;
			end
		else if(selection==1)
			begin
				if((word[0]==1&word[1]==1) | (word[1]==1&word[2]==1) | (word[2]==1&word[3]==1) | (word[3]==1&word[4]==1))
					begin
						if(mode==1)
							begin
								if(nerdians1==2)
									begin
										nerdians0=0;
										nerdians1=0;
									end							
								else if (nerdians0==9)
									begin
										nerdians0=0;
										nerdians1=nerdians1+1;
									end
								else
									begin
										nerdians0=nerdians0+1;
									end
							end
						else
							begin
								if(nerdians0==0 & nerdians1==0) nerdians0=0;
								else if(nerdians0==0)
									begin
										nerdians1=nerdians1-1;
										nerdians0=9;
									end
								else nerdians0=nerdians0-1;
							end
					end
				else 	warning=1;
			end
	end
	
endmodule


