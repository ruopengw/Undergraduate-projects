module judge_control(clk,in_duration,in_co,song_done);
	input clk,in_co;
	input [5:0] in_duration;
	output reg song_done;
	
	reg state,nextstate;
	parameter CONTINUE=0,OVER=1;
	
	initial state=CONTINUE;
	
	always @(posedge clk)
		state=nextstate;
		
	always @(*)
		case(state)
			CONTINUE:
			begin
			song_done=0;
			if(in_duration==6'b000000) nextstate=OVER;
			else if(in_co) nextstate=OVER;
			else nextstate=CONTINUE;
			end
			
			OVER:
			begin
			song_done=1;
			if(~in_co) nextstate=CONTINUE;
			else if(in_duration==6'b000000) nextstate=OVER;
			else nextstate=CONTINUE;
			end
		endcase
	
endmodule
