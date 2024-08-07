//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once

#if !defined( NO_ENTITY_PREDICTION )
	//-----------------------------------------------------------------------------
	// Purpose: Wraps 32bit predictID to allow access and creation
	//-----------------------------------------------------------------------------
	class CPredictableId {
	public:
		// Construction
		CPredictableId();

		static void ResetInstanceCounters();

		// Is the Id being used
		bool IsActive() const;

		// Call this to set from data
		void Init( int player, int command, const char* classname, const char* module, int line );

		// Get player index
		int GetPlayer() const;
		// Get hash value
		int GetHash() const;
		// Get index number
		int GetInstanceNumber() const;
		// Get command number
		int GetCommandNumber() const;

		// Check command number
		//	bool			IsCommandNumberEqual( int testNumber ) const;

		// Client only
		void SetAcknowledged( bool ack );
		bool GetAcknowledged() const;

		// For conversion to/from integer
		int GetRaw() const;
		void SetRaw( int raw );

		char const* Describe() const;

		// Equality test
		bool operator==( const CPredictableId& other ) const;
		bool operator!=( const CPredictableId& other ) const;

	private:
		void SetCommandNumber( int commandNumber );
		void SetPlayer( int playerIndex );
		void SetInstanceNumber( int counter );

		// Encoding bits, should total 32
		struct bitfields {
			unsigned int ack : 1;     // 1
			unsigned int player : 5;  // 6
			unsigned int command : 10;// 16
			unsigned int hash : 12;   // 28
			unsigned int instance : 4;// 32
		} m_PredictableID;
	};

	// This can be empty, the class has a proper constructor
	ALWAYS_INLINE void NetworkVarConstruct( CPredictableId& x ) { }
#endif
