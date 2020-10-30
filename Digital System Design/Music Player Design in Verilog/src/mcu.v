module mcu(clk,reset,play_pause,next,play,song,reset_play,song_done);
	input play_pause,clk,reset,next,song_done;
	output play,reset_play;
	output[1:0] song;
	wire nextsong;
	
	control_mcu control(
		.reset(reset),
		.clk(clk),
		.play_pause(play_pause),
		.next(next),
		.song_done(song_done),
		.nextsong(nextsong),
		.reset_play(reset_play),
		.play(play)
	);
	
	counter_n #(.n(4), .counter_bits(2)) counter_n(
		.clk(clk),
		.en(nextsong),
		.r(reset),
		.q(song),
		.co()
	);
endmodule
