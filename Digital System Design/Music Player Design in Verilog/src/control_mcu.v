module control_mcu(reset,clk,play_pause,next,song_done,nextsong,reset_play,play);
	input reset,clk,song_done,next,play_pause;
	output reg nextsong,reset_play,play;
	
	parameter RESET=0,PAUSE=1,PLAY=2,NEXT=3;
	reg[1:0] state,nextstate;
	
	always@(posedge clk)
		if(reset) state=RESET; else state=nextstate;
		
	always@(*)
		case(state)
			RESET:
			begin
			play=0;
			nextsong=0;
			reset_play=1;
			nextstate=PAUSE;
			end
			
			PAUSE:
			begin
			play=0;
			nextsong=0;
			reset_play=0;
			if(play_pause) nextstate=PLAY; 
			else if(next) nextstate=NEXT; 
			else nextstate=PAUSE;
			end
			
			PLAY:
			begin
			play=1;
			nextsong=0;
			reset_play=0;
			if(play_pause) nextstate=PAUSE; 
			else if(next) nextstate=NEXT; 
			else if(song_done) nextstate=RESET; 
			else nextstate=PLAY;
			end
			
			NEXT:
			begin
			play=0;
			nextsong=1;
			reset_play=1;
			nextstate=PLAY;
			end
		endcase

endmodule
