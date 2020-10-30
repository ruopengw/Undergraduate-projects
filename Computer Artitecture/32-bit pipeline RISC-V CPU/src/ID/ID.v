`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
//////////////////////////////////////////////////////////////////////////////////
module ID(clk,Instruction_id, PC_id, RegWrite_wb, rdAddr_wb, RegWriteData_wb, MemRead_ex, 
          rdAddr_ex, MemtoReg_id, RegWrite_id, MemWrite_id, MemRead_id, ALUCode_id, 
			 ALUSrcA_id, ALUSrcB_id,  Stall, Branch, Jump, IFWrite,  JumpAddr, Imm_id,
			 rs1Data_id, rs2Data_id,rs1Addr_id,rs2Addr_id,rdAddr_id);
    input clk;
    input [31:0] Instruction_id;
    input [31:0] PC_id;
    input RegWrite_wb;
    input [4:0] rdAddr_wb;
    input [31:0] RegWriteData_wb;
    input MemRead_ex;
    input [4:0] rdAddr_ex;
	
    output MemtoReg_id;
    output RegWrite_id;
    output MemWrite_id;
    output MemRead_id;
    output [3:0] ALUCode_id;
    output ALUSrcA_id;
    output [1:0] ALUSrcB_id;
    output Stall;
    output Branch;
    output Jump;
    output IFWrite;
    output [31:0] JumpAddr;
    output [31:0] Imm_id;
    output [31:0] rs1Data_id;
    output [31:0] rs2Data_id;
	output [4:0] rs1Addr_id,rs2Addr_id,rdAddr_id;

//**************************************
//Decode module
//**************************************
	wire JALR;
	wire [31:0] offset;
	
	Decode Decode_id(
	.MemtoReg(MemtoReg_id), 
	.RegWrite(RegWrite_id), 
	.MemWrite(MemWrite_id), 
	.MemRead(MemRead_id),
	.ALUCode(ALUCode_id),
	.ALUSrcA(ALUSrcA_id),
	.ALUSrcB(ALUSrcB_id),
	.Jump(Jump),
	.JALR(JALR),
	.Imm(Imm_id),
	.offset(offset),
    .Instruction(Instruction_id));

//**************************************
//Register module
//**************************************
	assign rs1Addr_id = Instruction_id[19:15];
	assign rs2Addr_id = Instruction_id[24:20];
	assign rdAddr_id = Instruction_id[11:7];
	
	RAW_Registers Registers(
	.rs1Data_id(rs1Data_id), 
	.rs2Data_id(rs2Data_id),  
	.clk(clk), 
	.WriteData_wb(RegWriteData_wb), 
	.WriteAddr_wb(rdAddr_wb), 
	.RegWrite_wb(RegWrite_wb), 
	.rs1Addr(rs1Addr_id), 
	.rs2Addr(rs2Addr_id));

//**************************************
//Hazard Detector
//**************************************

	Hazard_detector Hazard_detector_id(
	.rdAddr_ex(rdAddr_ex), 
	.MemRead_ex(MemRead_ex), 
	.rs1Addr_id(rs1Addr_id), 
	.rs2Addr_id(rs2Addr_id),
	.IFWrite(IFWrite),
	.Stall(Stall));

//**************************************
//Branch Test
//**************************************
	wire [2:0] funct3;
	assign funct3 = Instruction_id[14:12];
	wire SB_type;
	assign SB_type = (Instruction_id[6:0]==7'h63);
	
	Branch_test Branch_test_id(
	.SB_type(SB_type), 
	.funct3(funct3), 
	.rs1Data(rs1Data_id), 
	.rs2Data(rs2Data_id),
	.Branch(Branch));
 
//**************************************
//Jump
//**************************************
	wire [31:0] mux_out;
	mux2to1 #(.n(32)) mux2to1_id(
	.out(mux_out),
	.in0(PC_id),
	.in1(rs1Data_id),
	.addr(JALR));

	adder_32bits adder_32bits_id(
	.a(mux_out),
	.b(offset),
	.ci(1'b0),
	.s(JumpAddr),
	.co());

endmodule
