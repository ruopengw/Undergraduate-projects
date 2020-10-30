module cache_controller(clk, reset, ld, st, addr, l2_ack,
						tag_loaded_set1, tag_loaded_set2, valid_set1, valid_set2, dirty_set1, dirty_set2,
						//input
						hit, miss, load_ready, write_l1, read_l2, write_l2, state
						//output
						);
	input ld, st, reset, clk;
	input [31:0] addr;
	input [20:0] tag_loaded_set1, tag_loaded_set2;
	input valid_set1, valid_set2, dirty_set1, dirty_set2, l2_ack;
	output reg hit, miss, load_ready, write_l1, read_l2, write_l2;
	output reg [2:0] state;

	parameter s0=3'b000, s1=3'b001, s2=3'b010, s3=3'b011, s4=3'b100, s5=3'b101, s6=3'b110;
	
	reg [2:0] nextstate;
	reg valid_set1_active, valid_set2_active, dirty_set1_active, dirty_set2_active;
	wire valid, dirty, hit_block1;
	wire [2:0] count;

	assign hit_block1 = (addr[31:11] == tag_loaded_set1);
	assign valid = hit_block1 ? valid_set1 : valid_set2;
	assign dirty = hit_block1 ? dirty_set1 : dirty_set2;
	
	counter_n #(.n(8),.counter_bits(3)) counter_8(
		.clk(clk),
		.en(write_l2||read_l2),
		.r((miss && ld && !dirty)||(hit && ld && !valid)||(miss && ld && dirty)),
		.q(count),
		.co()); 
	
	
	always @(posedge clk)
		begin
			if(reset) state = s0;
			else state = nextstate;
		end
	
	always @(*)
		case(state)
			s0:  //Initialize
			begin
				load_ready=0;
				write_l1=0;
				read_l2=0;
				write_l2=0;
				dirty_set1_active=0;
				dirty_set2_active=0;
				valid_set1_active=0;
				valid_set2_active=0;
				if(!ld && !st) nextstate = s0;
				else if(ld||st) nextstate = s1;
			end
			
			s1:  //Compare_tag
			begin
				hit = (addr[31:11]==tag_loaded_set1)||(addr[31:11]==tag_loaded_set2);
				miss = (addr[31:11]!=tag_loaded_set1)&&(addr[31:11]!=tag_loaded_set2);
				load_ready=0;
				write_l1=0;
				read_l2=0;
				write_l2=0;
				dirty_set1_active=0;
				dirty_set2_active=0;
				valid_set1_active=0;
				valid_set2_active=0;
				if(hit && ld && valid) nextstate = s2;
				else if(hit && st) nextstate = s3;
				else if((miss && ld && !dirty)||(hit && ld && !valid)) nextstate = s4;
				else if(miss && st) nextstate = s5;
				else if(miss && ld && dirty) nextstate = s6;
			end
			
			s2:  //Idle_ld
			begin
				load_ready=1;
				if(!ld && !st) nextstate = s2;
				else if(ld || st) nextstate = s1;
				else if(ld && st) nextstate = s0;
			end
			
			s3:  //Idle_st
			begin
				write_l1=1;
				if(hit_block1) dirty_set1_active=1; 
				else dirty_set2_active=1;
				if(!ld && !st) nextstate = s3;
				else if(ld || st) nextstate = s1;
				else if(ld && st) nextstate = s0;
			end
			
			s4: //Allocate
			begin
				read_l2=l2_ack;
				if(count!=3'b111) 
					nextstate = s4;
				else
					begin 
					nextstate = s2; 
					if(hit_block1) valid_set1_active=1; 
					else valid_set2_active=1;
					end
			end
			
			s5:  //Idle_write
			begin
				write_l2=1;
				if(!ld && !st) nextstate = s5;
				else if(ld || st) nextstate = s1;
				else if(ld && st) nextstate = s0;
			end
			
			s6:  //Write_back
			begin
				write_l2=1;
				if(count!=3'b111) nextstate = s6;
				else begin nextstate = s4; write_l2=0; end
			end
			
			
		endcase
	
endmodule


module counter_n(clk, en, r, q, co);  
	parameter n=1; 
	parameter counter_bits=1;
	input clk, en, r;  
	output co;  
	output [counter_bits-1:0] q;  
	reg [counter_bits-1:0] q=0;  
	assign co=(q==(n-1))&&en;  
	always @(posedge clk)
		begin   
		if(r) 
			q=0; 
		else if(en) 
			begin 
			if(q==(n-1)) q=0; 
			else q=q+1; 
			end  
		end
endmodule