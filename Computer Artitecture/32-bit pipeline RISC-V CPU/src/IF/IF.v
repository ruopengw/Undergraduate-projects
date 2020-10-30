`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:  zju
// Engineer: qmj
//////////////////////////////////////////////////////////////////////////////////
module IF(clk, reset, Branch,Jump, IFWrite, JumpAddr,Instruction_if,PC,IF_flush);
    input clk;
    input reset;
    input Branch;
    input Jump;
    input IFWrite;
    input [31:0] JumpAddr;
    output [31:0] Instruction_if;
    output [31:0] PC;
    output IF_flush;

//******************************************************************************
// PC_mux
//******************************************************************************
	assign IF_flush = Jump||Branch;
	
	wire [31:0] NextPC_if;
	reg [31:0] PC_init;
	
    always @(*)
        begin
			case(Jump||Branch)
				1'b0: PC_init = NextPC_if;
				1'b1: PC_init = JumpAddr;
            endcase 
        end 


//******************************************************************************
// Instruction ROM
//******************************************************************************	
	dffre #(.WIDTH(32)) PC_reg(
        .d(PC_init),
        .en(IFWrite),
        .r(reset),
        .clk(clk),
        .q(PC));
		
	InstructionROM  InstructionROM_IF(
		.addr(PC[7:2]), //PC/4
		.dout(Instruction_if));

//******************************************************************************
// Adder
//******************************************************************************
	adder_32bits addr_if(
        .a(PC),
        .b(32'd4),
        .ci(1'b0),
        .s(NextPC_if),
        .co());


endmodule
