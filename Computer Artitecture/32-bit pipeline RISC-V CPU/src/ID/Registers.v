`timescale 1ns / 1ps
module Registers (
	// Outputs
	ReadData1, ReadData2,  
	// Inputs
	clk, WriteData, WriteRegister, RegWrite, ReadRegister1, ReadRegister2
);

	input clk;
	input [31:0] WriteData;
	input [4:0] WriteRegister;
	input RegWrite;
	input [4:0]	ReadRegister1, ReadRegister2;

	output [31:0] ReadData1;
	output [31:0] ReadData2;

	// Memory declaration 32*32
	reg [31:0]	regs [31:0];

	// Read from port 1/2
	assign ReadData1 = (ReadRegister1 == 5'b0)? 32'b0 : regs[ReadRegister1];
	assign ReadData2 = (ReadRegister2 == 5'b0)? 32'b0 : regs[ReadRegister2];
	
	// Write data
	always @ (posedge clk) 
		begin
			if (RegWrite)
				regs[WriteRegister] <= WriteData;
		end

endmodule