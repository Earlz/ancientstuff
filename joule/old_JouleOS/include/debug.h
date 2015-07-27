//PUBLIC DOMAIN
#ifndef DEBUG_H
#define DEBUG_H


#ifdef NDEBUG
#define debug if(1)
#else
#define debug if(0)
#endif


/*
used like
debug{
	printf("Were in debug mode!");
}else{
	printf("Were not in debug mode!");
}
*/














#endif