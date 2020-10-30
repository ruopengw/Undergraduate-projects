//******************************************************************************
// MIPS verilog model
//
// ALU.v
//******************************************************************************

module ALU (
	// Outputs
	   ALUResult,
	// Inputs
	   ALUCode, A, B);
	input [3:0]	ALUCode;				// Operation select
	input [31:0]	A, B;
	output reg [31:0]	ALUResult;

	
// Decoded ALU operation select (ALUsel) signals
   parameter	 alu_add=  4'b0000;
   parameter	 alu_sub=  4'b0001;
   parameter	 alu_lui=  4'b0010;
   parameter	 alu_and=  4'b0011;
   parameter	 alu_xor=  4'b0100;
   parameter	 alu_or =  4'b0101;
   parameter 	 alu_sll=  4'b0110;
   parameter	 alu_srl=  4'b0111;
   parameter	 alu_sra=  4'b1000;
   parameter	 alu_slt=  4'b1001;
   parameter	 alu_sltu= 4'b1010; 	


	wire Binvert;
	wire [31:0] sum, B_in, newBin;
	
	assign Binvert = ~(ALUCode==alu_add); //control +/-
	assign newBin = {32{Binvert}}; //after extension
	assign B_in = B^newBin;
	
	adder_32bits alu_addr(
	.a(A), 
	.b(B_in), 
	.ci(Binvert), 
	.s(sum), 
	.co());
	
	wire [31:0] d2_lui, d3_and, d4_xor, d5_or, d6_sll, d7_srl, d8_sra, d9_slt, d10_sltu;
	reg signed [31:0] A_reg;
	always @(A) 
		begin
			A_reg = A;
		end
	
	assign d2_lui = B;
	assign d3_and = A&B;
	assign d4_xor = A^B;
	assign d5_or = A|B;
	assign d6_sll = A<<B;
	assign d7_srl = A>>B;
	assign d8_sra = A_reg>>>B;
	assign d9_slt = (A[31]&&(~B[31]))||((A[31]~^B[31])&&sum[31]);
	assign d10_sltu = ((~A[31])&&B[31])||~((A[31]^B[31])&&sum[31]);

// ALU Result
	always @(*)
	begin
		case(ALUCode)
			alu_add:	ALUResult = sum;
			alu_sub:	ALUResult = sum;
			alu_lui:	ALUResult = d2_lui;
			alu_and:	ALUResult = d3_and;
			alu_xor:	ALUResult = d4_xor;
			alu_or :	ALUResult = d5_or;
			alu_sll:	ALUResult = d6_sll;
			alu_srl:	ALUResult = d7_srl;
			alu_sra:	ALUResult = d8_sra;
			alu_slt:	ALUResult = d9_slt;
			alu_sltu:	ALUResult = d10_sltu;
		endcase
	end
	

endmodule