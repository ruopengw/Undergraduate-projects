module adder_4bits(a,b,ci,s,co);
	input [3:0] a,b;
	input ci;
	wire [3:0] G,P;
	wire [2:0] C;
	output reg [3:0] s;
	output wire co;
	
	assign G[3]=a[3]&b[3];
	assign G[2]=a[2]&b[2];
	assign G[1]=a[1]&b[1];
	assign G[0]=a[0]&b[0];
	
	assign P[3]=a[3]|b[3];
	assign P[2]=a[2]|b[2];
	assign P[1]=a[1]|b[1];
	assign P[0]=a[0]|b[0];
	
	assign C[0]=G[0]|(P[0]&ci);
	assign C[1]=G[1]|(P[1]&G[0])|(P[1]&P[0]&ci);
	assign C[2]=G[2]|(P[2]&G[1])|(P[2]&P[1]&G[0])|(P[2]&P[1]&P[0]&ci);
	assign co=G[3]|(P[3]&G[2])|(P[3]&P[2]&G[1])|(P[3]&P[2]&P[1]&G[0])|(P[3]&P[2]&P[1]&P[0]&ci);
	
	always @(*)
		begin
			s[0]=(P[0]&~G[0])^ci;
			s[1]=(P[1]&~G[1])^C[0];
			s[2]=(P[2]&~G[2])^C[1];
			s[3]=(P[3]&~G[3])^C[2];
		end
			
endmodule
			
	
	