#if !defined(MODES_HPP)
#define MODES_HPP

struct ClientOperator {
	bool	isOperator;  // if enabled the client is an operator
} ;

struct ChannelModes {

	ChannelModes () {		
		isInviteOnly = 0;
		isUsersLimit = 1;
		isTopic = 0;
		isKey = 0;
	}
	ChannelModes (bool key) {		
		isInviteOnly = 0;
		isUsersLimit = 1;
		isTopic = 0;
		isKey = key;
	}
	bool	isInviteOnly; // if enabled channel is invite only
	bool	isUsersLimit; // if enabled channel has a user limit
	bool	isTopic; //  if enabled channel has mode topic enabled
	bool	isKey;  // if enabled password is required to join channel
} ;

#endif // MODES_HPP
