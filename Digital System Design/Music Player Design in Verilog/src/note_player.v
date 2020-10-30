module note_player(clk,reset,play_enable,note_to_load,duration_to_load,load_new_note,note_done,sampling_pulse,beat,sample);
	input clk,reset,play_enable,load_new_note,sampling_pulse,beat;
	input [5:0] note_to_load,duration_to_load;
	output note_done;
	output[15:0]sample;
	
	wire timer_clear_wire,timer_done_wire,load_wire;
	
	note_player_control player_control(
		.clk(clk),
		.reset(reset),
		.play_enable(play_enable),
		.load_new_note(load_new_note),
		.timer_clear(timer_clear_wire),
		.timer_done(timer_done_wire),
		.note_done(note_done),
		.load(load_wire)
	);
	
	
	beat_timer beat_timer(
		.clk(clk),
		.beat(beat),
		.duration_to_load(duration_to_load),
		.timer_clear(timer_clear_wire),
		.timer_done(timer_done_wire)
	);
	
	wire rin;
	wire [5:0] qout;
	assign rin=reset||(~play_enable);
	
	dffre #(.n(6)) dffre_con(
		.r(rin),
		.d(note_to_load),
		.en(load_wire),
		.clk(clk),
		.q(qout)
	);
	
	wire[19:0] dout_wire;
	
	frequency_rom FreqROM(
		.clk(clk),
		.dout(dout_wire),
		.addr(qout)
	);
	
	dds DDS(
		.clk(clk),
		.reset(rin),
		.k({2'b00,dout_wire}),
		.sampling_pulse(sampling_pulse),
		.sample(sample),
		.new_sample_ready()
	);
	
endmodule
