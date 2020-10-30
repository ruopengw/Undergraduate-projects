module beat_timer(beat,duration_to_load,clk,timer_clear,timer_done);
	input beat,clk,timer_clear;
	input[5:0]duration_to_load;
	output timer_done;
	
	wire [5:0]qout;
	wire aeb,rin;
	
	assign rin=timer_clear;
	assign timer_done=aeb&&beat;
	
	counter_n #(.n(64), .counter_bits(6)) beat_counter(
		.clk(clk),
		.en(beat),
		.r(rin),
		.q(qout),
		.co()
	);
	
	comp #(.n(6)) comp_beat(
		.a(duration_to_load-1),
		.b(qout),
		.alb(),
		.agb(),
		.aeb(aeb)
	);
	
endmodule
