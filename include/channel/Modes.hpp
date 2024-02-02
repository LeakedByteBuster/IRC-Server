#if !defined(MODES_HPP)
#define MODES_HPP

class ClientOperator {

private :
    virtual ChannelModes() = 0;

protected :
	bool	isOperator;  // if enabled the client is an operator

} ;

class ChannelModes {

private :
    virtual ChannelModes() = 0;

protected :
	bool	isInviteOnly; // if enabled channel is invite only
	bool	isUsersLimit; // if enabled channel has a user limit
	bool	isTopic; //  if enabled only operator can change topic
	bool	isKey;  // if enabled password is required to join channel

} ;

#endif // MODES_HPP
