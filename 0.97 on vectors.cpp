//by Gen0me https://github.com/63n0m3/mp3ext/
//btc: bc1qla2nm4zgnmvnedl5z4n8cqpvf5fjqrrh2akeec

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
using namespace std;

void set0bit(char * byte,int pos){
	int extra_bytes;
	extra_bytes=((pos-(pos%8))/8);
	pos=(pos%8);
	byte=byte+extra_bytes;
	switch(pos){
	case(7): if((unsigned int)(*byte)%2==1){
		(*byte)-=0x1;
	}
		return;
	case(6): if((unsigned int)(*byte)%4>=2){
			(*byte)-=0x2;
	}
			return;
	case(5): if((unsigned int)(*byte)%8>=4){
		(*byte)-=0x4;
	}
			return;
    case(4): if((unsigned int)(*byte)%16>=8){
    	(*byte)-=0x8;
    }
    		return;
	case(3): if((unsigned int)(*byte)%32>=16){
		(*byte)-=0x10;
	}
			return;
    case(2): if((unsigned int)(*byte)%64>=32){
    	(*byte)-=0x20;
    }
    		return;
	case(1): if((unsigned int)(*byte)%128>=64){
		(*byte)-=0x40;
	}
			return;
	case(0): if((unsigned int)(*byte)>=128){
		(*byte)-=0x80;
	}
			return;
		default:cout<<pos<<" set0bit wrong";

	}
}
void set0bits(char * byte,int pos,int length){
	int extra_bytes;
	extra_bytes=((pos-(pos%8))/8);
	pos=(pos%8);
	byte=byte+extra_bytes;
	while(pos%8!=0){
		set0bit(byte,pos);
		pos++;
		length--;
	}
	for(;length>=8;length-=8){
		*byte=(char)0x0;
		byte++;
	}
	if(length>0)for(int i=0;i<length;i++)set0bit(byte,i);
}

bool isbit(char * byte,int pos){
	int extra_bytes;
	extra_bytes=((pos-(pos%8))/8);
	pos=(pos%8);
	byte=byte+extra_bytes;
	switch(pos){
	case(7): if((unsigned int)(*byte)%2==1)return true;
	else return false;
	case(6): if((unsigned int)(*byte)%4>=2)return true;
	else return false;
	case(5): if((unsigned int)(*byte)%8>=4)return true;
	else return false;
    case(4): if((unsigned int)(*byte)%16>=8)return true;
	else return false;
	case(3): if((unsigned int)(*byte)%32>=16)return true;
	else return false;
    case(2): if((unsigned int)(*byte)%64>=32)return true;
	else return false;
	case(1): if((unsigned int)(*byte)%128>=64)return true;
	else return false;
	case(0): if((unsigned int)(*byte)>=128)return true;
	else return false;
	}
}

int slen_table(int sfc,int slen_no){
	if(slen_no==1){
	if(sfc<=3)return 0;
	else if(sfc==4||sfc==11||sfc==12||sfc==13)return 3;
	else if(sfc<=7)return 1;
	else if(sfc<=10)return 2;
	else if(sfc>=14)return 4;
	}
	if(slen_no==2){
		switch(sfc){
            case(0):return 0;
            case(1):return 1;
            case(2):return 2;
            case(3):return 3;
            case(4):return 0;
            case(5):return 1;
            case(6):return 2;
            case(7):return 3;
            case(8):return 1;
            case(9):return 2;
            case(10):return 3;
            case(11):return 1;
            case(12):return 2;
            case(13):return 3;
            case(14):return 2;
            case(15):return 3;
        }
    }
};

int bitrate_table(int mpeg,int layer, bool bit0,bool bit1,bool bit2,bool bit3){
	int added_bits=bit3+(2*bit2)+(4*bit1)+(8*bit0);
	if (added_bits==0)return 0;
		if (mpeg==1){
			if(layer==3){
				if(added_bits<6) return (8*added_bits+24);
				if(added_bits<10) return (16*added_bits-16);
                else if(added_bits<14) return (32*added_bits-160);
                if(added_bits==14) return (320);
            }
            else if(layer==2){
                if(added_bits<5)return (24+8*added_bits);
                else if(added_bits<9)return (16*added_bits-64);
                else if(added_bits<13)return (32*added_bits-128);
                else if(added_bits==13)return 320;
                else if(added_bits==14)return 384;
            }
            else if(layer==1){
                 if(added_bits<15)return (32*added_bits);
            }
		}

    else if(mpeg>=2){
		if(layer==3||layer==2){
            if(added_bits<9)return (8*added_bits);
            else if(added_bits<15)return (16*added_bits-64);
		}
		if(layer==1){
            if(added_bits==1)return 32;
            else if(added_bits<5)return (8*added_bits+32);
            else if(added_bits<13)return (16*added_bits);
            else if(added_bits<15)return (32*added_bits-192);
		}
	}
    cout<<"wrong bitrate"<<bit0<<bit1<<bit2<<bit3<<endl;
    return 0;
}

int bin_to_int(char * byte_pointer, int bit_start,int length){
	int tmp=0;
	for(int n=0;n<length;n++){
	tmp=tmp*2+(isbit(byte_pointer,bit_start+n));
	}
	return tmp;
}

int granule_size(char * byte_pointer, int bits){
	return (isbit(byte_pointer,bits + 11) + (2 * isbit(byte_pointer,bits + 10)) + (4 * isbit(byte_pointer,bits + 9))
        + ( 8 * isbit(byte_pointer,bits + 8)) + (16 * isbit(byte_pointer,bits + 7)) + (32 * isbit(byte_pointer,bits + 6))
        + (64 * isbit(byte_pointer,bits + 5)) + (128 * isbit(byte_pointer,bits + 4)) + (256 * isbit(byte_pointer,bits + 3))
        + (512 * isbit(byte_pointer,bits + 2)) + (1024 * isbit(byte_pointer,bits + 1)) + (2048 * isbit(byte_pointer,bits)));
}

void display_B(char *pointer, int from, int to){
	for(int i=from;i<to;i++){
		if(*(pointer+i)<16)cout<<"0";
        cout<<std::hex<<(int)*(pointer+i)<<" ";
	}
    cout<<std::dec;
}

struct frame{
	int gran1a;
	int gran1b;
	int gran2a;
	int gran2b;

	char * s_i;
	int m_s;
	bool padd;
	int i_point;
	int n_offset;
	int header_size;
	int frame_size;
	int shorten_flb;
	int flb;		//change to next_header_B
	short valid_frame;
	short dont_move_next_frame;
	int bitrate;
	int B_pre_valid;
	int smpl_rate;
	int empty_end_b;
	short mpeg_sampling;
	short is_layer;
	int valid_frame_index;     //should be constructed with 0

	short sfc1a;
	short sfc1b;
	short sfc2a;
	short sfc2b;
	bool mixed_block1a;
	short block_type1a;
	bool mixed_block1b;
	short block_type1b;
	bool mixed_block2a;
	short block_type2a;
	bool mixed_block2b;
	short block_type2b;
	int p2l_1a;  //part 2 length
	int p2l_1b;
	int p2l_2a;
	int p2l_2b;

	int table_length(int slen1,int slen2,bool mixed_block,int block_type){
        if(block_type==2){
			if(mixed_block==0)return (18*slen1 + 18*slen2);
			else return (17*slen1 + 18*slen2);
		}
		else return (11*slen1 + 10*slen2);
	}
	void find_frame_param(char * file_data){
		if(isbit(file_data+i_point,11)==false && isbit(file_data+i_point,12)==false){
            mpeg_sampling=4;
       	}
        else if(isbit(file_data+i_point,11)==true && isbit(file_data+i_point,12)==false){
            mpeg_sampling=2;
        }
        else if(isbit(file_data+i_point,11)==true && isbit(file_data+i_point,12)==true)mpeg_sampling=1;

        if(isbit(file_data+i_point,13)==false && isbit(file_data+i_point,14)==true){
            is_layer=3;
        }
        else if(isbit(file_data+i_point,13)==true && isbit(file_data+i_point,14)==false){
        	is_layer=2;
        }
        else if(isbit(file_data+i_point,13)==true && isbit(file_data+i_point,14)==true){
        	is_layer=1;
        }
        if(isbit(file_data+i_point,20)==false && isbit(file_data+i_point,21)==false)smpl_rate=44100;
        else if(isbit(file_data+i_point,20)==false && isbit(file_data+i_point,21)==true)smpl_rate=48000;
        else if(isbit(file_data+i_point,20)==true && isbit(file_data+i_point,21)==false)smpl_rate=32000;
        smpl_rate=smpl_rate/mpeg_sampling;

        if(isbit(file_data+i_point,15)==false){
        //CRC on
            s_i=file_data;
            s_i=s_i+i_point+16+4;
            header_size=20;
        }
        else{
        //	CRC off
            s_i=file_data;
            s_i=s_i+i_point+4;
            header_size=4;
        }
          	////
        n_offset = (isbit(s_i,8))+(2 * isbit(s_i,7))+(4 * isbit(s_i,6))+(8 * isbit(s_i,5))
            + (16 * isbit(s_i,4)) + (32 * isbit(s_i,3)) + (64 * isbit(s_i,2))+(128 * isbit(s_i,1))+(256 * isbit(s_i,0));
        bitrate = bitrate_table(mpeg_sampling, is_layer, isbit(file_data + i_point,16),
            isbit(file_data + i_point,17), isbit(file_data + i_point,18), isbit(file_data + i_point,19));

        flb=(144000*bitrate)/smpl_rate;

          //layer 3&2 only
        if(isbit(file_data+i_point,22)==true){
           padd=1;
           flb++;
        }
        else padd=0;
        ////
        if(isbit(file_data+i_point,24)==true && isbit(file_data+i_point,25)==true ){
            m_s=1;
            header_size+=17;
            gran1a=granule_size(s_i,18);
            gran2a=granule_size(s_i,18+59);
			gran1b=0;
			gran2b=0;

            if (isbit(s_i,51)==false){  //win sw
                block_type1a=0;
                mixed_block1a=0;		//???
			}
			else{
                block_type1a=bin_to_int(s_i,52,2);
                mixed_block1a=isbit(s_i,54);
            }
            if (isbit(s_i,110)==false){
					block_type2a=0;
					mixed_block2a=0;		//???
            }
            else{
                block_type2a=bin_to_int(s_i,111,2);
                mixed_block2a=isbit(s_i,113);
            }
            sfc1a=bin_to_int(s_i,37,4);
            sfc2a=bin_to_int(s_i,37+59,4);
        }
        else {
            m_s=2;
            header_size+=32;
            gran1a=granule_size(s_i,20);
        	gran1b=granule_size(s_i,20+59);
        	gran2a=granule_size(s_i,138);
        	gran2b=granule_size(s_i,138+59);

            if (isbit(s_i,53)==false){  //win sw
                block_type1a=0;
                mixed_block1a=0;		//???
			}
			else{
                block_type1a=bin_to_int(s_i,54,2);
                mixed_block1a=isbit(s_i,56);
            }

            if (isbit(s_i,112)==false){
                block_type1b=0;
                mixed_block1b=0;		//???
            }
            else{
                block_type1b=bin_to_int(s_i,113,2);
                mixed_block1b=isbit(s_i,115);
            }

            if (isbit(s_i,171)==false){  //win sw
                block_type2a=0;
                mixed_block2a=0;		//???
			}
			else{
                block_type2a=bin_to_int(s_i,172,2);
                mixed_block2a=isbit(s_i,174);
            }
            if (isbit(s_i,230)==false){
                block_type2b=0;
                mixed_block2b=0;		//???
            }
            else{
                block_type2b=bin_to_int(s_i,231,2);
                mixed_block2b=isbit(s_i,233);
            }

            sfc1a=bin_to_int(s_i,39,4);
            sfc1b=bin_to_int(s_i,39+59,4);
            sfc2a=bin_to_int(s_i,39+118,4);
            sfc2b=bin_to_int(s_i,39+118+59,4);
        }
		p2l_1a = table_length( slen_table(sfc1a,1), slen_table(sfc1a,2), mixed_block1a, block_type1a);
		p2l_1b = table_length( slen_table(sfc1b,1), slen_table(sfc1b,2), mixed_block1b, block_type1b);
		p2l_2a = table_length( slen_table(sfc2a,1), slen_table(sfc2a,2), mixed_block2a, block_type2a);
        p2l_2b = table_length( slen_table(sfc2b,1), slen_table(sfc2b,2), mixed_block2b, block_type2b);

        if((gran1a + gran1b + gran2a + gran2b) % 8 == 0) frame_size = header_size + (gran1a + gran1b + gran2a + gran2b) / 8;
        else frame_size = header_size + (gran1a + gran1b + gran2a + gran2b) / 8 + 1;
	}

		void display_param(){
		cout<<std::dec<<i_point<<"i  "<<bitrate<<"kbps "<<smpl_rate<<" "<<flb<<"flb "<<n_offset<<"n_off "<<B_pre_valid<<"pre_h_valid  "<<padd<<"pad "<<frame_size<<"fr_s "<<m_s<<"m/s  "<<valid_frame<<"val "<<empty_end_b<<"empty_end_b"<<endl;
	}

	void display(char * source){
		cout<<std::dec<<header_size<<"header "<<" gran1a:"<<gran1a<<" gran1b:"<<gran1b<<" gran2a:"<<gran2a<<" gran2b:"<<gran2b<<endl;
        for(int i= (-1)*n_offset; i<flb+2; i++){
            if(n_offset==0){
                if(i==0)cout<<"frame start - header: ";
                if(i==header_size)cout<<"gran 1a: ";
            }
            else{
                if (i== (-1)* n_offset)cout<<"frame start granule 1a: ";
                if(i==0)cout<<"header: ";
            }

			if(n_offset<=(gran1a/8)){
                if (i==(gran1a/8)-n_offset+header_size){
                    if(m_s==2)cout<<"gran 1b: *";
                    else cout<<"gran 2a: *";
                }
            }
            else if (i==(gran1a/8)-n_offset){
				if(m_s==2)cout<<"gran 1b: *";
				else cout<<"gran 2a: *";
            }
            if (n_offset<((gran1a+gran1b+gran2a)/8)){
                if (i == ((gran1a + gran1b + gran2a) / 8) - n_offset + header_size){
                    if(m_s==2)cout<<"gran 2b: *";
                    else cout<<"gran 2a: *";
                }
            }
            else if (i==((gran1a+gran1b+gran2a)/8)-n_offset){
				if(m_s==2)cout<<"gran 2b: *";
				else cout<<"gran 2a: *";
			}

            if (m_s==2){
                if(n_offset<((gran1a+gran1b)/8)){
                    if (i==((gran1a+gran1b)/8)-n_offset+header_size)cout<<"gran 2a: *";
                }
                else if (i==((gran1a+gran1b)/8)-n_offset)cout<<"gran 2a: *";
            }
            if(i == - n_offset + header_size + (gran1a + gran1b + gran2a + p2l_2b) / 8) cout<<"#";
            if(((int)*(source+i))<16)cout<<"0";
            cout<<std::hex<<(int)*(source+i)<<" ";
            if (i == ((gran1a + gran1b + gran2a + gran2b) / 8) - n_offset + header_size) cout<<"* <- end of granules  ";
            if(n_offset != 0 && i == header_size-1)cout<<" <- end of header  ";
		//recheck all this may me missed by 1Byte
        }
	cout<<endl;
	}

	void move_data(char * source, int header_distance, int move_frame){	//frame size dependent!
        if(header_distance > frame_size - header_size)cout<<"Wrong header_distance"<<endl;
		char data[frame_size-header_size];
		char header[header_size];
		for(int n=0;n<header_size;n++)header[n]=*(source+i_point+n);

		int k=0;
		for(int n=0;n<frame_size;n++){
			if(n==n_offset)n=n+header_size;
			data[k]=*(source+i_point-n_offset+n);
			k++;
		}
		int h=0;
		int d=0;
		for(int n=0;n<frame_size;n++){
			if(n>=header_distance&&n<header_size+header_distance){
				*(source+i_point-n_offset+n+move_frame)=header[h];
				h++;
            }
			else {
                *(source+i_point-n_offset+n+move_frame)=data[d];
                d++;
			}
		}

		i_point=i_point-n_offset+header_distance+move_frame;
		s_i=s_i-(n_offset-header_distance+move_frame);
		n_offset=header_distance;
		*s_i=header_distance/2;
		*(s_i+1)=*(s_i+1)&127;
		*(s_i+1)+=(header_distance%2)*128;
	}
};

void frames_stats(struct frame * fr,int frame_count, char * source, bool show_only_valid){
	int stat_table[4][4][16][4][4]={0};
	int most_often[15][6]={0};
	int tmp=0xfffffff-1;
		//mpeg,layer,bitrate,smpl freq,mode
	for(int n=0;n<frame_count;n++){
		if(1-show_only_valid||fr[n].valid_frame){
            if(bin_to_int(source+fr[n].i_point, 11,2)==1)continue;
            if(bin_to_int(source+fr[n].i_point, 13,2)==0)continue;
            if(bin_to_int(source+fr[n].i_point+2, 0,4)==15)continue;
            if(bin_to_int(source+fr[n].i_point, 20,2)==3)continue;
            stat_table [bin_to_int(source + fr[n].i_point, 11,2)] [bin_to_int(source + fr[n].i_point, 13,2)]
                [bin_to_int(source + fr[n].i_point, 16,4)] [bin_to_int(source + fr[n].i_point, 20,2)]
                [bin_to_int(source + fr[n].i_point, 24,2)] ++;
        }
	}

    if(show_only_valid==0)cout<<std::dec<<"Most often frames out of "<<frame_count<<":"<<endl;
	else cout<<std::dec<<"Valid frames only:"<<endl;
	for(int i=0;i<20;i++){
		for(int k=0;k<4;k++){
			for(int l=0;l<4;l++){
				for(int m=0;m<16;m++){
					for(int n=0;n<4;n++){
						for(int o=0;o<4;o++){
                            if( (stat_table[k][l][m][n][o] > most_often[i][0] && stat_table[k][l][m][n][o] < most_often[i-1][0] && i > 0)
                               || (stat_table[k][l][m][n][o] > most_often[i][0] && i == 0) ){
                                most_often[i][0]=stat_table[k][l][m][n][o];
                                most_often[i][1]=k;
                                most_often[i][2]=l;
                                most_often[i][3]=m;
                                most_often[i][4]=n;
                                most_often[i][5]=o;
                            }
		}}}}}
		if(most_often[i][0]==0)break;
		cout<<most_often[i][0]<<")	Mpeg ";
		if(3==(int)most_often[i][1])cout<<"1 ";
		if(0==(int)most_often[i][1])cout<<"2.5 ";
		if(2==most_often[i][1])cout<<"2 ";
		if(most_often[i][2]==1)cout<<"layer 3 ";
		if(most_often[i][2]==2)cout<<"layer 2 ";
		if(most_often[i][2]==3)cout<<"layer 1 ";
		cout<<bitrate_table(4 - most_often[i][1], 4 - (most_often[i][2]), isbit((char*) & most_often[i][3],4),
            isbit((char*) & most_often[i][3],5), isbit((char*) & most_often[i][3],6), isbit((char*) & most_often[i][3],7))
            <<"kbps ";
        int smpl_rate;
        if(most_often[i][4]==0)smpl_rate=44100;
        else if(most_often[i][4]==1)smpl_rate=48000;
        else if(most_often[i][4]==2)smpl_rate=32000;
        most_often[i][1]=4-most_often[i][1];
        cout<<smpl_rate/most_often[i][1]<<" ";
		if(most_often[i][5]==0)cout<<"stereo	";
		if(most_often[i][5]==1)cout<<"joint st	";
		if(most_often[i][5]==2)cout<<"dual ch	";
		if(most_often[i][5]==3)cout<<"mono	";
		cout<<endl;
    }
	cout<<endl;
}

int check_frames(struct frame * fr,int frame_count){
    int valid_counter=0;
    int vbr_counter=0;
    int next_frame_found;
	fr[0].valid_frame=1;
	//check for id tags
    for(int n=0;n<frame_count;n++){
        next_frame_found=0;
        if(fr[n].valid_frame==1&&fr[n].bitrate !=0){
            for(int k=1;k<100;k++)if(n+k<frame_count){

                if((fr[n].flb+fr[n].i_point==fr[n+k].i_point)/*&&(fr[n].frame_size+fr[n].i_point-fr[n].n_offset<=3+fr[n+k].i_point-fr[n+k].n_offset)*/){
                    fr[n+k].valid_frame=1;
                    next_frame_found=1;
                    fr[n].empty_end_b = ( (fr[n + k].i_point-fr[n + k].n_offset-(fr[n].i_point - fr[n].n_offset))
                        - fr[n].header_size ) * 8 - ( fr[n].gran1a + fr[n].gran1b + fr[n].gran2a + fr[n].gran2b );

                    fr[n+k].B_pre_valid=fr[n+k].i_point-fr[n].i_point;
                    if(fr[n].empty_end_b<0)fr[n].dont_move_next_frame=1;
                    else fr[n].dont_move_next_frame=0;
                    break;
                }
		}}
        if(fr[n].valid_frame==1&&next_frame_found==0/*&&fr[n].bitrate!=0*/){
			for(int k=1;k<100;k++)if(n+k<frame_count){
                if(fr[n].frame_size + fr[n].i_point - fr[n].n_offset < fr[n + k].i_point - fr[n + k].n_offset
                    && fr[n].flb + fr[n].i_point < fr[n + k].i_point){
                        fr[n+k].valid_frame=1;
                        next_frame_found=1;
                        fr[n].empty_end_b = ((fr[n + k].i_point - fr[n + k].n_offset - (fr[n].i_point - fr[n].n_offset))
                            - fr[n].header_size) * 8 - (fr[n].gran1a + fr[n].gran1b + fr[n].gran2a + fr[n].gran2b);
                        fr[n+k].B_pre_valid=fr[n+k].i_point-fr[n].i_point;
                        cout<<endl<<endl<<"WRONG2: "<<n<<" "<<n + k<<" "<<(fr[n].i_point - fr[n].n_offset) - (fr[n + k].i_point
                            - fr[n + k].n_offset) - fr[n].frame_size<<endl;
                        break;
                }
            }
		}
    }
    for(int n=0;n<frame_count;n++){
		if(fr[n].gran1a + fr[n].gran1b + fr[n].gran2a + fr[n].gran2b == 0)fr[n].valid_frame = 0;
		if(fr[n].valid_frame==1){
			valid_counter++;
			fr[n].valid_frame_index=valid_counter;
        }
    }
    return valid_counter;
}

/// This function looks for frame headers that fit mp3 model. each frame header starts with a specified in mp3 specification bit signature,
/// And than based on consequent bits some container characteristics are determined. Not every option is allowed so this function checks
/// for different possible container types, if something doesnt fit, just skips and continues search.
/// This is just an initial check, in fact this function finds many false headers as in usual mp3 file there is plenty of data that fits first 31 bits.
/// True frame headers keep info (flb aka "frame length buffer") which tells where to find next header.
/// Those long long will allow for using files bigger than 2.15 GB
/// It takes pointer to char* data and length of the data and returns vector of indexes to location of potential frames (source+return[n]) is the location of each
vector<long long> potential_frames(char * source,long long length){
    vector <long long> headers_i;
    for(long long i=0;i<length-4;i++){
        if(*(source+i) == (char)0xff && isbit(source+i,8) == true &&  isbit(source+i,9) == true && isbit(source+i,10) == true){
            if(isbit(source+i,11) == false && isbit(source+i,12) == true){
        	//	wrong_mpeg
                continue;
            }
            if(isbit(source+i,13) == false && isbit(source+i,14) == false){
     //     	wrong_layer
                continue;
          	}
           if( isbit(source+i,16) == true && isbit(source+i,17) == true && isbit(source+i,18) == true
              && isbit(source+i,19) == true){
              	//wrong bitrate
              	continue;
            }
            if(isbit(source+i,20) == true && isbit(source+i,21) == true){
       //   	wrong_sampling11
                continue;
          	}
            if(isbit(source+i,30) == true && isbit(source+i,31) == false){
         // 	wrong_emphasis
                continue;
            }
			headers_i.push_back(i);
        }
    }
    return headers_i;
}

void repack_frames(struct frame * fr,int frame_count,char * source){
	for(int n=0;n<frame_count;n++){
		if(fr[n].valid_frame==1){
			set0bits(source+fr[n].i_point-fr[n].n_offset,fr[n].gran1a+fr[n].gran1b+fr[n].gran2a+fr[n].gran2b+(fr[n].header_size*8),fr[n].empty_end_b);
        }
}}
    /// This is a kind of a debug check. Extracted frames are rewritten back again to new file for check of valid frames. Not all tracks will work, especially ones overleapingly packed.
long long rewrite_frames(struct frame * fr, int frames_count, char * input, char * output){
    long s=0;
    int tmp=0;
    long s1;
    int k_i;
    int end_bits;
    int neg_offset_fr0 = fr[0].n_offset;
    if(neg_offset_fr0 < 0) neg_offset_fr0 = 0;

    for(int n = 0; n < frames_count; n++){
        if(fr[n].valid_frame == 0) continue;
            if(fr[n].empty_end_b < 0){
                for(int k = 0;n + k < frames_count && k < 100; k++){
                    if(fr[n + k].valid_frame == 1){
                        end_bits += (fr[n + k].i_point - fr[n + k].n_offset) - (fr[n].i_point - fr[n].n_offset);
                        if(fr[n + k].empty_end_b < end_bits) end_bits = fr[n + k].empty_end_b;
    	   //					s=s+fr[n].n_offset;
    	   				for(int r = - fr[n].n_offset; r < (fr[n + k].i_point - fr[n + k].n_offset) - fr[n].i_point; r++){
    	   					*(output + s + r + neg_offset_fr0) = *(input + fr[n].i_point + r);
    	   				}
    	   				s = s + fr[n + k].i_point - fr[n].i_point;
                        break;
                    }
            }}
            else
                for (int r = - fr[n].n_offset; r < fr[n].frame_size - fr[n].n_offset; r++){
                    *(output + s + r + neg_offset_fr0) = *(input + fr[n].i_point + r);
                }
            s = s + fr[n].flb;
    }
    return s + neg_offset_fr0;
}

int main(){

    int end_cin;
	int unopened_mp3_files = 0;
	string file_name="21. Gareth Emery - Tokyo.mp3";
	long cleaned_size;
	streampos beg_file,end_file;
	char * file_data;
	char *nd_point;
	char *fd_point;
	char input;
	int tmp;
	int valid_frames = 0;
	bool show_only_valid_frames_sw = 0;
	bool show_frame_data_sw = 0;
	int adjust_first_dis_fr = 140;
	int adjust_displayed_frames = 60;
/*	char * nd_point;
	char * fd_point;	*/
	string new_file_name;
	string cfg_str;
	ifstream cfg ("cfg.txt", ios::binary);
	cfg.seekg(0, ios::beg);
    if(cfg.is_open()){
    	getline(cfg,cfg_str);
		getline(cfg,file_name);
    	sscanf(cfg_str.c_str(),"%d %d %d %d", & show_only_valid_frames_sw, & adjust_first_dis_fr, & adjust_displayed_frames, & show_frame_data_sw);
        cfg.close();
    }
    else cout<<"Couldnt open cfg file, cfg file will be created"<<endl;
	cout<<"Last opened file(any)/Input file name(i)/Show only valid frames on(v)/Show frame data(s)/Adjust displayed frames(a)/Exit(e):"<<endl;
	while(1){
        cin>>input;
		if(input == 'v'){
            if(show_only_valid_frames_sw == 0){
                show_only_valid_frames_sw = 1;
                cout<<"Show only valid frames ON"<<endl;
            }
            else{
                show_only_valid_frames_sw = 0;
                cout<<"Show only valid frames OFF"<<endl;
            }
        }
		else if(input == 'a'){
			cout<<"First displayed frame: ";
			cin>>adjust_first_dis_fr;
			cout<<endl<<"Displayed frames: ";
			cin>>adjust_displayed_frames;
		}
		else if(input == 's'){
            if(show_frame_data_sw == 0){
                show_frame_data_sw = 1;
                cout<<"Show frame data ON"<<endl;
            }
            else{
                show_frame_data_sw=0;
                cout<<"Show frame data OFF"<<endl;
            }
		}
		else break;
    }
    if(input == 'e');
    else{
		if(input == 'i'){
            cout<<"File name, patch:";
            std::getline(std::cin >> std::ws, file_name);
        }
       	ofstream cfg ("cfg.txt", ios::binary); cfg<<show_only_valid_frames_sw<<"	"<<adjust_first_dis_fr<<"	"<<adjust_displayed_frames<<"	"<<show_frame_data_sw<<endl<<file_name;
        cfg.close();

        ifstream file (file_name.c_str(), ios::binary);
        beg_file = file.tellg();
        file.seekg(0, ios::end);
        end_file = file.tellg();

        if(file.is_open()){
            file_data = new char[end_file-beg_file];
            file.seekg(0, ios::beg);
            file.read(file_data,end_file-beg_file);

            cout<<file_name<<" file opened. "<<"File size is: "<<(end_file-beg_file)<<" Bytes. "<<"File data read."<<endl;
            char * new_data = new char[end_file - beg_file + 5000];
            vector<long long> headers_indexes;
            headers_indexes = potential_frames(file_data, end_file - beg_file);
          	struct frame * f1 = new frame[headers_indexes.size()];
            for(long n = 0 ; n < headers_indexes.size(); n++){
                f1[n].i_point = headers_indexes[n];
                f1[n].find_frame_param(file_data);
            }
            frames_stats(f1, headers_indexes.size(), file_data,0);
            valid_frames = check_frames(f1,headers_indexes.size());
            frames_stats(f1,headers_indexes.size(), file_data, 1);
            if(adjust_first_dis_fr + adjust_displayed_frames >= headers_indexes.size()){
                cout<<"adjust_displayed_frames reached over last frame"<<endl;
                adjust_displayed_frames = (headers_indexes.size()) - adjust_first_dis_fr;
            }
            for(int i = adjust_first_dis_fr; i < adjust_displayed_frames + adjust_first_dis_fr; i++) if(f1[i].valid_frame == 1 || show_only_valid_frames_sw == 0){
                cout<< std::dec << i << "/" << f1[i].valid_frame_index << ") ";
                cout<< f1[i].i_point + f1[i].flb << "i+flb  ";
                f1[i].display_param();
                if(show_frame_data_sw) f1[i].display(file_data + f1[i].i_point);
            }
            for(int i = 0; i<file_name.find(".mp3"); i++)new_file_name = new_file_name + file_name[i];
            new_file_name = new_file_name + "_extr.mp3";
            ofstream clean (new_file_name.c_str(), ios::binary);
            if(clean.is_open());
            else {
                cout<< "Couldnt create " << new_file_name << endl ;
            }
            long long s = rewrite_frames(f1, headers_indexes.size(), file_data, new_data);
            for(long long i = 0; i < s; i++) clean << *(new_data + i);
            cout<<endl<<endl;
            cleaned_size = clean.tellp();
            clean.close();
            delete[] f1;
         //   delete[] new_data;
            delete[] file_data;
        }
        else{
            cout<<"Uneable to open mp3 file"<<endl;
            unopened_mp3_files++;
        }
        cout<<"final frames: "<<std::dec<<valid_frames<<endl<<"frames length(s) of valid frames: "<<valid_frames*1152/41000<<"s = "<<((valid_frames*1152/41000)-(valid_frames*1152/41000)%60)/60<<"min "<<(valid_frames*1152/41000)%60<<"s (for layer 2 or 3 frames)"<<endl;
        cout<<file_name<<"	"<<end_file<<" Bytes"<<endl;
        cout<<new_file_name<<"	"<<cleaned_size<<" Bytes"<<endl;
	}
    cout<<endl<<endl<<"by Gen0me"<<endl;
	cin>>end_cin;
	return 0;
}
