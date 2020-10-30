module Hazard_detector(
	//input
	rdAddr_ex, MemRead_ex, rs1Addr_id, rs2Addr_id,
	//output
	IFWrite, Stall
);
	input MemRead_ex;
	input [4:0]	rs1Addr_id, rs2Addr_id;
	input [4:0] rdAddr_ex;
	output IFWrite, Stall;
	
	assign Stall = ((rdAddr_ex==rs1Addr_id)||(rdAddr_ex==rs2Addr_id))&&MemRead_ex;
	assign IFWrite = ~Stall;

endmodule