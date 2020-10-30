module music_player #(parameter sim=0)(
	input clk,
	input reset,
	input play_pause,
	input next,
	input NewFrame,
	output [15:0] sample,
	output play,
	output [1:0] song);
	
	//主控制器
	wire reset_play;
	wire song_done;
	mcu MCU(
		.clk(clk),
		.reset(reset),
		.play_pause(play_pause),
		.next(next),
		.play(play),
		.song(song),
		.reset_play(reset_play),
		.song_done(song_done)); //song_done
	
	//同步化电路
	wire ready;
	syn_circuit SYN(
		.sys_clk(clk),
		.in(NewFrame),
		.out(ready));
	
	//节拍基准产生器
	wire beat;
	beat_base BB(
		.ready(ready),
		.sys_clk(clk),
		.beat(beat));
		
	//乐曲读取
	wire [5:0] note;
	wire [5:0] duration;
	wire new_note;
	wire note_done;
	song_reader SR(
		.clk(clk),
		.reset(reset_play),
		.play(play),
		.song(song),
		.song_done(song_done),
		.note(note),
		.duration(duration),
		.new_note(new_note),
		.note_done(note_done));
		
	//音符播放
	note_player NP(
		.clk(clk),
		.reset(reset_play),
		.play_enable(play),
		.note_to_load(note),
		.duration_to_load(duration),
		.load_new_note(new_note),
		.note_done(note_done),
		.sampling_pulse(ready),
		.beat(beat),
		.sample(sample));
	
endmodule
	
	
	
	
	
	
	