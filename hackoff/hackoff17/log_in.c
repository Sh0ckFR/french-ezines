 /**********************************************************
Envoie les infos necessaires pour se logguer au serveur IRC
**********************************************************/ 

/* On passe le numero de socket comme argument */
void log_in(int sock)
{
    /* On utilise un petit buffer */
    char buff[512];
    /* Le format est : USER <username> <hostname> <servername> <realname>
                       NICK <nickname> */
    sprintf(buff, "USER fred . . Ucender\nNICK Ucebot\n");
    /* Et on envoie le tout */
    send(sock, buff, strlen(buff), 0);
}
