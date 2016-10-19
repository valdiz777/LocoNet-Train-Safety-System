#include <iostream>

#ifndef TRACKINFO_H
#define TRACKINFO_H

class TrackInfo
{
	public:
		TrackInfo();
		~TrackInfo();

		void setStatus( bool status );
		bool getStatus(  );

		void setPosition( std::string position );
		std::string getPosition(  );
};

#endif
