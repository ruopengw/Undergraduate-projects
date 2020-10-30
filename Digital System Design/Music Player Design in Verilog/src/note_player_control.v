module note_player_control(clk,reset,play_enable,load_new_note,timer_clear,timer_done,note_done,load);
	input clk,reset,play_enable,load_new_note,timer_done;
	output reg note_done,timer_clear,load;
	
	reg[1:0]state,nextstate;
	parameter RESET=0,PLAY=1,DONE=2,LOAD=3;
	
	always @(posedge clk)
		if(reset) state=RESET; else state=nextstate;
	
	always @(*)
		case(state)
			RESET:
			begin
			timer_clear=1;
			load=0;
			note_done=0;
			nextstate=PLAY;
			end
			
			PLAY:
			begin
			timer_clear=0;
			load=0;
			note_done=0;
			if(~play_enable) nextstate=RESET; 
			else if(timer_done) nextstate=DONE; 
			else if(load_new_note) nextstate=LOAD; 
			else nextstate=PLAY;
			end
			
			DONE:
			begin
			timer_clear=1;
			load=0;
			note_done=1;
			nextstate=PLAY;
			end
			
			LOAD:
			begin
			timer_clear=1;
			load=1;
			note_done=0;
			nextstate=PLAY;
			end
		endcase
	
	
endmodule
