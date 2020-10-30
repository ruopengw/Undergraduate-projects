module RAW_Registers (
	// Outputs
	rs1Data_id, rs2Data_id,  
	// Inputs
	clk, WriteData_wb, WriteAddr_wb, RegWrite_wb, rs1Addr, rs2Addr
);

	input clk;
	input [4:0]	rs1Addr, rs2Addr;
	input [4:0]	WriteAddr_wb;
	input RegWrite_wb;
	input [31:0] WriteData_wb;
	
	output [31:0] rs1Data_id; // data output for read port 1
	output [31:0] rs2Data_id; // data output for read port 2
	
	wire rs1Sel, rs2Sel;
	wire [31:0] ReadData1, ReadData2;
	
	//transfer detection
	assign rs1Sel = RegWrite_wb&&((WriteAddr_wb!=0))&&(WriteAddr_wb==rs1Addr);
	assign rs2Sel = RegWrite_wb&&((WriteAddr_wb!=0))&&(WriteAddr_wb==rs2Addr);
	
	Registers RBWRegisters(
		//output
		.ReadData1(ReadData1),
		.ReadData2(ReadData2),
		//input
		.clk(clk), 
		.WriteData(WriteData_wb), 
		.WriteRegister(WriteAddr_wb), 
		.RegWrite(RegWrite_wb), 
		.ReadRegister1(rs1Addr), 
		.ReadRegister2(rs2Addr));
	
	mux2to1 #(.n(32)) mux1(
        .out(rs1Data_id),
        .in0(ReadData1),
        .in1(WriteData_wb),
        .addr(rs1Sel));
	
	mux2to1 #(.n(32)) mux2(
        .out(rs2Data_id),
        .in0(ReadData2),
        .in1(WriteData_wb),
        .addr(rs2Sel));

endmodule