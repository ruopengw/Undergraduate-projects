module Branch_test(
	//input
	SB_type, funct3, rs1Data, rs2Data,
	//output
	Branch
);
	input SB_type;
	input [2:0] funct3;
	input [31:0] rs1Data, rs2Data;
	output reg Branch;
	wire [31:0] sum;
	wire isLT, isLTU;
	
	parameter beq_funct3 = 3'b000;
	parameter bne_funct3 = 3'b001;
	parameter blt_funct3 = 3'b100;
	parameter bge_funct3 = 3'b101;
	parameter bltu_funct3 = 3'b110;
	parameter bgeu_funct3 = 3'b111;
	
	// rs1Data - rs2Data
	adder_32bits adder_sum(
	.a(rs1Data),
	.b(~rs2Data),
	.ci(1'b1),
	.s(sum[31:0]),
	.co());
	
	assign isLT = rs1Data[31]&&(~rs2Data[31])||(rs1Data[31]~^rs2Data[31])&&sum[31];
	assign isLTU = (~rs1Data[31])&&rs2Data[31]||(rs1Data[31]~^rs2Data[31])&&sum[31];
	
	always @(*)
	begin	
		if(SB_type==1'b1)
			begin
				case(funct3)
					beq_funct3: Branch = ~(|sum[31:0]);
					bne_funct3: Branch = |sum[31:0];
					blt_funct3: Branch = isLT;
					bge_funct3: Branch = ~isLT;
					bltu_funct3: Branch = isLTU;
					bgeu_funct3: Branch = ~isLTU;
					default: Branch = 1'b0;
				endcase
			end
		else
			begin Branch = 1'b0; end
	end

endmodule