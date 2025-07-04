#ifndef PROTOCOL_H
#define PROTOCOL_H

typedef enum {
    CONNECT,    // Request: CONNECT <nickname>                              Response: CONNECT <true/false> <id> <token> 
    COUNT,      // Request: COUNT <type> <token>                            Response: COUNT <true/false> <type> <count>
    GET,        // Request: GET <type> <range number> <token>               Response: GET <true/false> <type> <list>
    CREATE,     // Request: CREATE <type> <name> <token>                    Response: CREATE <true/false> <type> <id> <name>
    JOIN,       // Request: JOIN <type> <id> <token>                        Response: JOIN   <true/false> <type> <id>
    ACCEPT,     // Request: ACCEPT <id_lobby> <id_player> <token>           Response: ACCEPT <true/false> <type> <id>
    START_GAME, // Request: START_GAME <id_player1> <id_player2> <token>    Reponse: START_GAME <true/false> <id_game>
    MOVE,       // Request: MOVE <id_game> <id_player> <ind_x> <ind_y>      Response: MOVE  <true/false>
    BOARD_UPDATE, //                                                        Response: BOARD_UPDATE <id_game> <id_player> <ind_x> <ind_y>
    RESIGN,     // Request: RESIGN <id_game> <id_player>                    Response RESIGN <true/false> <id_game> <id_player>
    GAME_OVER,  //                                                          Response GAME_OVER <id_game> <win/lose/draw> <winner/loser>
    REMATCH,    // Request: REMATCH <from_id_player> <to_id_player>         Response REMATCH <true/false>
} msg_t;

#endif