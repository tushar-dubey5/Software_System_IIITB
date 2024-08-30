/*
============================================================================
Name : 2.c
Author : Tushar Dubey
Description : Write a simple program to execute in an infinite loop at the background. Go to /proc directory and
				identify all the process related information in the corresponding proc directory
Date: 30th Aug, 2024.
============================================================================
*/

int main(){
	for(;;);
return 1;
}
//Here is the output after running cat /proc/38683/status where 38683 is the PID of the process
// Name:	Question1
// Umask:	0002
// State:	R (running)
// Tgid:	38683
// Ngid:	0
// Pid:	38683
// PPid:	26786
// TracerPid:	0
// Uid:	1000	1000	1000	1000
// Gid:	1000	1000	1000	1000
// FDSize:	256
// Groups:	4 24 27 30 46 100 118 1000 
// NStgid:	38683
// NSpid:	38683
// NSpgid:	38683
// NSsid:	26786
// VmPeak:	    6176 kB
// VmSize:	    6176 kB
// VmLck:	       0 kB
// VmPin:	       0 kB
// VmHWM:	    2944 kB
// VmRSS:	    2944 kB
// RssAnon:	     128 kB
// RssFile:	    2816 kB
// RssShmem:	       0 kB
// VmData:	     264 kB
// VmStk:	     132 kB
// VmExe:	       4 kB
// VmLib:	    3504 kB
// VmPTE:	      52 kB
// VmSwap:	       0 kB
// HugetlbPages:	       0 kB
// CoreDumping:	0
// THP_enabled:	1
// Threads:	1
// SigQ:	0/31070
// SigPnd:	0000000000000000
// ShdPnd:	0000000000000000
// SigBlk:	0000000000000000
// SigIgn:	0000000000000000
// SigCgt:	0000000000000000
// CapInh:	0000000000000000
// CapPrm:	0000000000000000
// CapEff:	0000000000000000
// CapBnd:	000001ffffffffff
// CapAmb:	0000000000000000
// NoNewPrivs:	0
// Seccomp:	0
// Seccomp_filters:	0
// Speculation_Store_Bypass:	thread vulnerable
// SpeculationIndirectBranch:	conditional enabled
// Cpus_allowed:	ff
// Cpus_allowed_list:	0-7
// Mems_allowed:	00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000001
// Mems_allowed_list:	0
// voluntary_ctxt_switches:	0
// nonvoluntary_ctxt_switches:	803
