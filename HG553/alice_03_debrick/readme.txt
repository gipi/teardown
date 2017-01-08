**************************************************************************

  Alice03.c - EJTAG Debrick Utility for Alice routers v0.3

  Heavily based on the WRT54G/GS EJTAG Debrick Utility  v4.5/v4.8
  and on Telsey MAGIC EJTAG Debrick Utility v0.9

**************************************************************************
  Written by 30252783, Janniz, HairyDairyMaid, cattivo12, beghiero
**************************************************************************


========================================
History
========================================

  v 0.1 - Adattamento dell'utilità di debrick di Janniz per supportare,
          oltre al Telsey Magic, Pirelli Vela e Pirelli AGA
  v 0.2 - Aggiunto il supporto al Pirelli AGB; corretti tutti i problemi
          di endianness
  v 0.3 - Aggiunto il supporto al Pirelli AGPF

========================================
Uso in Windows
========================================

1. Installare GiveIO (v. oltre)

2. Al prompt di DOS digitare

     Alice03 [parameter] [switches]

   dove al posto di [parameter], necessario, ci può essere

     -backup:<cfe|cfe_nvram|bootimg1|bootimg2|psi_nvram|scratch_pad|psi|wholeflash|custom>	per eseguire il backup della parte di flash
     -erase:<cfe|cfe_nvram|bootimg1|bootimg2|psi_nvram|scratch_pad|psi|wholeflash|custom>	per cancellare la parte flash
     -flash:<cfe|cfe_nvram|bootimg1|bootimg2|psi_nvram|scratch_pad|psi|wholeflash|custom>	per scrivere sulla parte di flash
     -probeonly											per testare l'interfaccia
     -show:fc											per visualizzare l'elenco delle flash supportate

   e al posto di [switches] (facoltativi) ci possono essere

     /noreset 				per saltare l'EJTAG CPU reset
     /noemw 				per saltare l'enabling memory writes
     /nocwd 				per saltare il clearing CPU watchdog timer
     /nobreak				per saltare l'issuing debug mode JTAGBRK
     /noerase				per non cancellare la flash prima di scriverla
     /notimestamp			per non aggiungere data e ora al nome del file creato con il backup backup
     /dma 				per forzare l'uso della modalità DMA (più veloce, opzione di default)
     /nodma				per forzare l'uso della modalità PrACC (più lenta ma più compatibile)
     /window:XXXXXXXX 			per impostare una flash window personalizzata (in HEX)
     /start:XXXXXXXX			per impostare un indirizzo iniziale personalizzato (in HEX)
     /length:XXXXXXXX			per impostare una lunghezza personalizzata (in HEX)
     /silent				per non visualizzare a schermo i dati letti o scritti
     /skipdetect			per saltare il riconoscimento del processore
     /instrlen:XX			per impostare manualmente l'instruction length del processore
     /cable:<xilinx|jkeys|wiggler>	per selezionare il tipo di cavo (default xilinx)
     /fc:XX				per selezionare manualmente un modello di flash
                                    	(utilizzare -show:fc per visualizzare l'elenco delle possibilità)

Iniziare con un Alice03 -probeonly (più lo switch di selezione del tipo di cavo se non si usa uno xilinx) per verificare
se tutto funziona correttamente.

L'operazione di backup produce un file binario il cui nome è composto dal nome della porzione di flash letta più data e ora
di creazione (a meno che non si usi lo switch /notimestamp).

Ad esempio per eseguire il backup di CFE con un cavo xilinx utilizzare il comando
  Alice03 -backup:cfe
se invece si utilizza un cavo wiggler il comando diventa
  Alice03 -backup:cfe /cable:wiggler

L'operazione di flash richiede che sia presente nella directory del programma un file binario (.bin) chiamato come la porzione
di flash da scrivere.

Ad esempio per flashare CFE, mettere il file cfe.bin nella cartella del programma e con un cavo xilinx utilizzare il comando
  Alice03 -flash:cfe
se invece si utilizza un cavo wiggler il comando diventa
  Alice03 -flash:cfe /cable:wiggler

Se si sceglie una porzione di flash custom occorre utilizzare gli switch windows, start e length.

========================================
Installare GiveIO in Windows
========================================

1. Lanciare il programma loaddrv.exe avendo cura che si trovi nella stessa directory sia presente giveio.sys.
   Selezionare l'opzione Install. Questo dovrebbe copiare il file giveio.sys in C:\WINDOWS\system32\drivers, se ciò
   non dovesse accadere (e probabilmente non accadrà) va copiato manualmente.

2. Selezionare in loaddrv il percorso completo del file giveio.sys (es: c:\windows\system32\drivers\giveio.sys).

3. Premere in loaddrv il pulsante Start.

4. Il driver a questo punto dovrebbe essere stato avviato correttamente. Se desiderate lanciare automaticamente il driver
   all'avvio di Windows procedere con i passi successivi.

5. Aprire Gestione periferiche e dal menu Visualizza selezionare Mostra periferiche nascoste. Tra le periferiche
   appariranno i Driver non Plug and Play.

6. Espandere i driver Non-Plug and Play.

7. Click destro sul driver giveio, selezionare Proprietà. Si aprirà la finestra delle proprietà del
   driver giveio.

8. Selezionare la linguetta Driver.

9. Nella lista di opzioni relativa Tipo di Avvio scegliere Automatico e confermare le modifiche premendo OK.

I cambiamenti avranno effetto dal prossimo riavvio di Windows.


========================================
Note
========================================

 - Il programma utilizza di default lo spazio di indirizzamento 0x1FC00000.

 - E' possibile testare facilmente altri spazi di indirizzamento utilizzando l'opzione backup:custom e gli switch window, start,
   length. Es:
     ./Alice03 -backup:custom /window:3fc00000 /start:403f0000 /length:10000 /nodma     
     
 - Dopo ogni operazione è necessario spegnere e accendere il router prima di potervi accedere nuovamente.
 
 - Questo programma è stato realizzato principalmente per permettere di recuperare router con il CFE corrotto. Benchè 
   sia possibile ricaricare completamente il contenuto della flash, è consigliabile utilizzare questo programma soltanto
   per recuperare il CFE e successivamente caricare il firmware con gli altri metodi normalmente a disposizione

 - Rispetto alla versione di Janniz ho eliminato la compatibilità con i WRT54G perché il meccanismo di scelta degli spazi di
   memoria, basato sulle dimensioni della flash, mal si adatta ai router Alice, che hanno tutti la stessa quantità di flash.
   Il nuovo meccanismo si basa su marca e modello di flash.
