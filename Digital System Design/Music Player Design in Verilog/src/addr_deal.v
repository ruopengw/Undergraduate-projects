module addr_deal(raw_addr,rom_addr);
	input [10:0] raw_addr; //20:10
	output [9:0] rom_addr; 
/*
	always @(raw_addr)
		begin
		if(raw_addr[10])
			begin
			if(raw_addr[10:0]==1024)
				rom_addr=1023;
			else
				rom_addr=~raw_addr+1;
			end
		else
			rom_addr=raw_addr;
		end
*/
	assign 
	rom_addr[9:0]=(raw_addr[10])?((raw_addr[10:0]==1024)?1023:(~raw_addr[9:0]+1)):raw_addr[9:0];
endmodule