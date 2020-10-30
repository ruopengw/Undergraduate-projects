`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: zju
// Engineer: qmj
//////////////////////////////////////////////////////////////////////////////////
module Risc5CPU(clk, reset, JumpFlag, Instruction_id, ALU_A, 
                     ALU_B, ALUResult_ex, PC, MemDout_mem,Stall);
    input clk;
    input reset;
    output [1:0] JumpFlag;
    output [31:0] Instruction_id;
    output [31:0] ALU_A;
    output [31:0] ALU_B;
    output [31:0] ALUResult_ex;
    output [31:0] PC;
    output [31:0] MemDout_mem;
    output Stall;

//**************************************
// IF MODEL
//**************************************
	wire Branch, Jump, IFWrite, IF_flush;
	wire [31:0] JumpAddr, Instruction_if;

	IF IF_model(
		.clk(clk), 
		.reset(reset), 
		.Branch(Branch),
		.Jump(Jump), 
		.IFWrite(IFWrite), 
		.JumpAddr(JumpAddr),
		.Instruction_if(Instruction_if),
		.PC(PC),
		.IF_flush(IF_flush));

	assign JumpFlag = {Jump,Branch};
	
//**************************************
// IF/ID DFF
//**************************************
	wire [31:0] PC_id;
	
	dffre #(.WIDTH(32)) DFF_nextpc_IF(
        .d(PC),
        .en(IFWrite),
        .r(IF_flush||reset),
        .clk(clk),
        .q(PC_id));
	
	dffre #(.WIDTH(32)) DFF_Instruction_IF(
        .d(Instruction_if),
        .en(IFWrite),
        .r(IF_flush||reset),
        .clk(clk),
        .q(Instruction_id)); 
	

//**************************************
// ID MODEL
//**************************************
	wire RegWrite_wb, MemRead_ex;
	wire [4:0] rdAddr_wb, rdAddr_ex, rs1Addr_id, rs2Addr_id, rdAddr_id;
	wire [31:0] RegWriteData_wb, Imm_id, rs1Data_id, rs2Data_id;
	wire MemtoReg_id, RegWrite_id, MemWrite_id, MemRead_id;
	wire [3:0] ALUCode_id;
	wire ALUSrcA_id;
	wire [1:0] ALUSrcB_id;

	ID ID_model(
		.clk(clk),
		.Instruction_id(Instruction_id), 
		.PC_id(PC_id), 
		.RegWrite_wb(RegWrite_wb), 
		.rdAddr_wb(rdAddr_wb), 
		.RegWriteData_wb(RegWriteData_wb), 
		.MemRead_ex(MemRead_ex), 
		.rdAddr_ex(rdAddr_ex), 
		.MemtoReg_id(MemtoReg_id), 
		.RegWrite_id(RegWrite_id), 
		.MemWrite_id(MemWrite_id), 
		.MemRead_id(MemRead_id), 
		.ALUCode_id(ALUCode_id), 
		.ALUSrcA_id(ALUSrcA_id), 
		.ALUSrcB_id(ALUSrcB_id),  
		.Stall(Stall), 
		.Branch(Branch), 
		.Jump(Jump), 
		.IFWrite(IFWrite),  
		.JumpAddr(JumpAddr), 
		.Imm_id(Imm_id),
		.rs1Data_id(rs1Data_id), 
		.rs2Data_id(rs2Data_id),
		.rs1Addr_id(rs1Addr_id),
		.rs2Addr_id(rs2Addr_id),
		.rdAddr_id(rdAddr_id));

//**************************************
// ID/EX DFF
//**************************************
	wire MemtoReg_ex, RegWrite_ex, MemWrite_ex;
	wire [3:0] ALUCode_ex;
	wire ALUSrcA_ex;
	wire [1:0] ALUSrcB_ex;
	wire [31:0] PC_ex, Imm_ex, rs1Data_ex, rs2Data_ex;
	wire [4:0] rs1Addr_ex, rs2Addr_ex;
	
	dffr #(.WIDTH(1)) DFF_MemtoReg_EX(
		.d(MemtoReg_id), 
		.r(Stall||reset), 
		.clk(clk), 
		.q(MemtoReg_ex));
	
	dffr #(.WIDTH(1)) DFF_RegWrite_EX(
		.d(RegWrite_id), 
		.r(Stall||reset), 
		.clk(clk), 
		.q(RegWrite_ex));

	dffr #(.WIDTH(1)) DFF_MemWrite_EX(
		.d(MemWrite_id), 
		.r(Stall||reset), 
		.clk(clk), 
		.q(MemWrite_ex));

	dffr #(.WIDTH(1)) DFF_MemRead_EX(
		.d(MemRead_id), 
		.r(Stall||reset), 
		.clk(clk), 
		.q(MemRead_ex));

	dffr #(.WIDTH(4)) DFF_ALUCode_EX(
		.d(ALUCode_id), 
		.r(Stall||reset), 
		.clk(clk), 
		.q(ALUCode_ex));

	dffr #(.WIDTH(1)) DFF_ALUSrcA_EX(
		.d(ALUSrcA_id), 
		.r(Stall||reset), 
		.clk(clk), 
		.q(ALUSrcA_ex));

	dffr #(.WIDTH(2)) DFF_ALUSrcB_EX(
		.d(ALUSrcB_id), 
		.r(Stall||reset), 
		.clk(clk), 
		.q(ALUSrcB_ex));
		
	dffr #(.WIDTH(32)) DFF_PC_EX(
		.d(PC_id), 
		.r(Stall||reset), 
		.clk(clk), 
		.q(PC_ex));

	dffr #(.WIDTH(32)) DFF_Imm_EX(
		.d(Imm_id), 
		.r(Stall||reset), 
		.clk(clk), 
		.q(Imm_ex));
		
	dffr #(.WIDTH(5)) DFF_rdAddr_EX(
		.d(rdAddr_id), 
		.r(Stall||reset), 
		.clk(clk), 
		.q(rdAddr_ex));
		
	dffr #(.WIDTH(32)) DFF_rs1Addr_EX(
		.d(rs1Addr_id), 
		.r(Stall||reset), 
		.clk(clk), 
		.q(rs1Addr_ex));

	dffr #(.WIDTH(32)) DFF_rs2Addr_EX(
		.d(rs2Addr_id), 
		.r(Stall||reset), 
		.clk(clk), 
		.q(rs2Addr_ex));

	dffr #(.WIDTH(32)) DFF_rs1Data_EX(
		.d(rs1Data_id), 
		.r(Stall||reset), 
		.clk(clk), 
		.q(rs1Data_ex));

	dffr #(.WIDTH(32)) DFF_rs2Data_EX(
		.d(rs2Data_id), 
		.r(Stall||reset), 
		.clk(clk), 
		.q(rs2Data_ex));
		
//**************************************
// EX MODEL
//**************************************
	wire [31:0] ALUResult_mem, ALUResult_ex, MemWriteData_ex;
	wire [4:0] rdAddr_mem;
	wire RegWrite_mem;

	EX EX_model(
		.ALUCode_ex(ALUCode_ex), 
		.ALUSrcA_ex(ALUSrcA_ex), 
		.ALUSrcB_ex(ALUSrcB_ex),
		.Imm_ex(Imm_ex), 
		.rs1Addr_ex(rs1Addr_ex), 
		.rs2Addr_ex(rs2Addr_ex), 
		.rs1Data_ex(rs1Data_ex), 
		.rs2Data_ex(rs2Data_ex), 
		.PC_ex(PC_ex), 
		.RegWriteData_wb(RegWriteData_wb), 
		.ALUResult_mem(ALUResult_mem),
		.rdAddr_mem(rdAddr_mem), 
		.rdAddr_wb(rdAddr_wb), 
		.RegWrite_mem(RegWrite_mem), 
		.RegWrite_wb(RegWrite_wb), 
		.ALUResult_ex(ALUResult_ex), 
		.MemWriteData_ex(MemWriteData_ex), 
		.ALU_A(ALU_A), 
		.ALU_B(ALU_B));


//**************************************
// EX/MEM DFF
//**************************************
	wire MemtoReg_mem, MemWrite_mem;
	wire [31:0] MemWriteData_mem;

	dffr #(.WIDTH(1)) DFF_MemtoReg_MEM(
		.d(MemtoReg_ex), 
		.r(reset), 
		.clk(clk), 
		.q(MemtoReg_mem));
	
	dffr #(.WIDTH(1)) DFF_RegWrite_MEM(
		.d(RegWrite_ex), 
		.r(reset), 
		.clk(clk), 
		.q(RegWrite_mem));

	dffr #(.WIDTH(1)) DFF_MemWrite_MEM(
		.d(MemWrite_ex), 
		.r(reset), 
		.clk(clk), 
		.q(MemWrite_mem));

	dffr #(.WIDTH(32)) DFF_ALUResult_MEM(
		.d(ALUResult_ex), 
		.r(reset), 
		.clk(clk), 
		.q(ALUResult_mem));
		
	dffr #(.WIDTH(32)) DFF_MemWriteData_MEM(
		.d(MemWriteData_ex), 
		.r(reset), 
		.clk(clk), 
		.q(MemWriteData_mem));

	dffr #(.WIDTH(5)) DFF_rdAddr_MEM(
		.d(rdAddr_ex), 
		.r(reset), 
		.clk(clk), 
		.q(rdAddr_mem));
		

//**************************************
// MEM MODEL
//**************************************
	DataRAM DataRAM_MEM(
		.a(ALUResult_mem[7:2]),
		.d(MemWriteData_mem),
		.clk(clk),
		.we(MemWrite_mem),
		.spo(MemDout_mem));


//**************************************
// MEM/WB DFF
//**************************************
	wire [31:0] MemDout_wb, ALUResult_wb;
	wire MemtoReg_wb;

	dffr #(.WIDTH(1)) DFF_MemtoReg_WB(
		.d(MemtoReg_mem), 
		.r(reset), 
		.clk(clk), 
		.q(MemtoReg_wb));
	
	dffr #(.WIDTH(1)) DFF_RegWrite_WB(
		.d(RegWrite_mem), 
		.r(reset), 
		.clk(clk), 
		.q(RegWrite_wb));

	dffr #(.WIDTH(32)) DFF_MemDout_WB(
		.d(MemDout_mem), 
		.r(reset), 
		.clk(clk), 
		.q(MemDout_wb));

	dffr #(.WIDTH(32)) DFF_ALUResult_WB(
		.d(ALUResult_mem), 
		.r(reset), 
		.clk(clk), 
		.q(ALUResult_wb));
		
	dffr #(.WIDTH(5)) DFF_rdAddr_WB(
		.d(rdAddr_mem), 
		.r(reset), 
		.clk(clk), 
		.q(rdAddr_wb));
		

//**************************************
// WB MODEL
//**************************************
	mux2to1 #(.n(32)) RegWriteData_mux(
		.out(RegWriteData_wb), 
		.in0(ALUResult_wb), 
		.in1(MemDout_wb), 
		.addr(MemtoReg_wb));
		

endmodule
