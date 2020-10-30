`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: zju
// Engineer: qmj
//////////////////////////////////////////////////////////////////////////////////
module EX(ALUCode_ex, ALUSrcA_ex, ALUSrcB_ex,Imm_ex, rs1Addr_ex, rs2Addr_ex, rs1Data_ex, 
          rs2Data_ex, PC_ex, RegWriteData_wb, ALUResult_mem,rdAddr_mem, rdAddr_wb, 
		  RegWrite_mem, RegWrite_wb, ALUResult_ex, MemWriteData_ex, ALU_A, ALU_B);
    input [3:0] ALUCode_ex;
    input ALUSrcA_ex;
    input [1:0] ALUSrcB_ex;
    input [31:0] Imm_ex;
    input [4:0]  rs1Addr_ex;
    input [4:0]  rs2Addr_ex;
    input [31:0] rs1Data_ex;
    input [31:0] rs2Data_ex;
	input [31:0] PC_ex;
    input [31:0] RegWriteData_wb;
    input [31:0] ALUResult_mem;
	input [4:0] rdAddr_mem;
    input [4:0] rdAddr_wb;
    input RegWrite_mem;
    input RegWrite_wb;
    output [31:0] ALUResult_ex;
    output [31:0] MemWriteData_ex;
    output reg [31:0] ALU_A;
    output reg [31:0] ALU_B;

//******************************************************************************
// 4 Mux Models
//******************************************************************************
	reg [31:0] MuxA0, MuxB0;
	wire [1:0] ForwardA, ForwardB;
		
	always@(*)
        begin
			case(ForwardA)
				2'b00: MuxA0 = rs1Data_ex;
				2'b01: MuxA0 = RegWriteData_wb;
				2'b10: MuxA0 = ALUResult_mem;
			endcase
        end 
    always@(*)
        begin
			case(ForwardB)
				2'b00: MuxB0 = rs2Data_ex;
				2'b01: MuxB0 = RegWriteData_wb;
				2'b10: MuxB0 = ALUResult_mem;
			endcase
        end 
    always@(*)
        begin
			case(ALUSrcA_ex)
				1'b0: ALU_A = MuxA0;
				1'b1: ALU_A = PC_ex;
			endcase
        end
    always@(*)
        begin
			case(ALUSrcB_ex)
				2'b00: ALU_B = MuxB0;
				2'b01: ALU_B = Imm_ex;
				2'b10: ALU_B = 32'd4;
			endcase
        end

	assign MemWriteData_ex = MuxB0;

//******************************************************************************
// ALU
//******************************************************************************
	ALU ALU_ex(
	.ALUResult(ALUResult_ex),
	.ALUCode(ALUCode_ex), 
	.A(ALU_A), 
	.B(ALU_B));

//******************************************************************************
// Forwarding
//******************************************************************************	
	forward forward_ex(
	.rs1Addr_ex(rs1Addr_ex), 
	.rs2Addr_ex(rs2Addr_ex), 
	.RegWrite_wb(RegWrite_wb), 
	.rdAddr_wb(rdAddr_wb), 
	.RegWrite_mem(RegWrite_mem), 
	.rdAddr_mem(rdAddr_mem),
	.ForwardA(ForwardA), 
	.ForwardB(ForwardB));
	

endmodule
