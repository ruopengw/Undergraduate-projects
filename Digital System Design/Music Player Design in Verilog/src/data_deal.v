module data_deal(area,raw_data,data);
	parameter N=16;
	input area;
	input [N-1:0] raw_data;
	output [N-1:0] data;
	assign data=area?(~raw_data+1):raw_data;
endmodule
