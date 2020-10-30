`timescale 1ns / 1ps

module cache_controller_tb;
	
	parameter delay=100;
	
	reg clk, reset, ld, st, valid_set1, valid_set2, dirty_set1, dirty_set2, l2_ack;
	reg [31:0] addr;
	reg [20:0] tag_loaded_set1, tag_loaded_set2;
	wire [2:0] state;
	wire hit, miss, load_ready, write_l1, read_l2, write_l2;
	
	cache_controller L1D(
		.clk(clk),
		.reset(reset),
		.ld(ld),
		.st(st),
		.addr(addr),
		.tag_loaded_set1(tag_loaded_set1),
		.tag_loaded_set2(tag_loaded_set2),
		.valid_set1(valid_set1),
		.valid_set2(valid_set2),
		.dirty_set1(dirty_set1),
		.dirty_set2(dirty_set2),
		.l2_ack(l2_ack),
		.hit(hit),
		.miss(miss),
		.load_ready(load_ready),
		.write_l1(write_l1),
		.read_l2(read_l2),
		.write_l2(write_l2),
		.state(state));
		
		
	initial 
	begin
		// Initialize Inputs
		clk = 0;
		reset = 1;
		ld = 0;
		st = 0;
		valid_set1 = 0;
		valid_set2 = 0;
		dirty_set1 = 0;
		dirty_set2 = 0;
		l2_ack = 0;
		addr = 32'b0;
		tag_loaded_set1 = 21'b0;
		tag_loaded_set2 = 21'b0;
		
		#(delay)  reset=0; //initialize
		#(delay) ld = 1; addr = 32'hffffffff; tag_loaded_set1 = 21'h1fffff; valid_set1 = 1; //read hit and valid
		#(delay*2) ld = 0; //idle
		#(delay*2)  ld = 1; valid_set1 = 0; l2_ack = 1; //read hit but invalid
		#(delay*2) ld = 0; st = 1; //write hit and valid
		#(delay*10) st = 1; tag_loaded_set1 = 21'h0fffff; //write miss
		#(delay*2) st = 0; ld = 1; dirty_set1 = 0; //read miss and the dirty bit is 0
		
		#(delay*8)  $stop;

	end

	always	#(delay/2) clk=~clk;

endmodule