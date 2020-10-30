module song_reader(clk,reset,play,song,song_done,note,duration,new_note,note_done);
	input clk,reset,play,note_done;
	input [1:0] song;
	output song_done,new_note;
	output[5:0] note, duration;
	wire[4:0] low_rom;
	wire co;
	
	counter_n #(.n(32),.counter_bits(5)) addr_counter(
		.clk(clk),
		.en(note_done),
		.r(reset),
		.q(low_rom),
		.co(co)
	);
	
	song_rom song_rom(
		.clk(clk),
		.dout({note,duration}),
		.addr({song,low_rom})
	);
	
	reader_control song_reader_control(
		.clk(clk),
		.reset(reset),
		.note_done(note_done),
		.play(play),
		.new_note(new_note)
	);
	
	judge_control judge(
		.clk(clk),
		.in_duration(duration),
		.in_co(co),
		.song_done(song_done)
	);
	
endmodule
