module reader_control(clk,reset,note_done,play,new_note);
	input clk,reset,note_done,play;
	output reg new_note;
	
	reg[1:0]state,nextstate;
	parameter RESET=0,NEW_NOTE=1,WAIT=2,NEXT_NOTE=3;
	
	always@(posedge clk)
		if(reset) state=RESET; else state=nextstate;
		
	always@(*)
		case(state)
			RESET:
			begin
			new_note=0;
			if(play) nextstate=NEW_NOTE; 
			else nextstate=RESET;
			end
			
			NEW_NOTE:
			begin
			new_note=1;
			nextstate=WAIT;
			end
			
			WAIT:
			begin
			new_note=0;
			if(!play) nextstate=RESET; 
			else if(note_done) nextstate=NEXT_NOTE; 
			else nextstate=WAIT;
			end
			
			NEXT_NOTE:
			begin
			new_note=0;
			nextstate=NEW_NOTE;
			end
		endcase
	
endmodule
