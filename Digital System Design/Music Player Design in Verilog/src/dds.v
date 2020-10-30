module dds(clk,reset,k,sampling_pulse,new_sample_ready,sample);
	input clk;
	input reset;
	input sampling_pulse;
	input [21:0] k;
	wire [21:0] addr;
	wire [21:0] raw_addr;
	wire [9:0] rom_addr;
	wire [15:0] raw_data;
	wire [15:0] data;
	wire area;
	output [15:0] sample;
	output new_sample_ready;
	
	phase_adder #(22) sum(
		.a(raw_addr[21:0]),
		.b(k[21:0]),
		.s(addr),
		.ci(0),
		.co());
		
	dffre #(.n(22)) dffre_addre(
		.d(addr),
		.en(sampling_pulse),
		.r(reset),
		.clk(clk),
		.q(raw_addr));
	
	dffre #(.n(1)) area_add(
		.d(raw_addr[21]),
		.en(1),
		.r(0),
		.clk(clk),
		.q(area));
	
	addr_deal addr_deal(
		.raw_addr(raw_addr[20:10]),
		.rom_addr(rom_addr));	

	sine_rom sine_rom(
		.clk(clk),
		.addr(rom_addr),
		.dout(raw_data));
	
	data_deal #(.N(16)) data_deal(
		.area(area),
		.raw_data(raw_data),
		.data(data));
		
	dffre #(.n(16)) data_adde(
		.d(data),
		.en(sampling_pulse),
		.r(0),
		.clk(clk),
		.q(sample));
	
	dffre #(.n(1)) ready_add(
		.d(sampling_pulse),
		.en(1),
		.r(0),
		.clk(clk),
		.q(new_sample_ready));

endmodule
	