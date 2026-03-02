import serial
import serial.tools.list_ports
import subprocess 
# Configuration
PORT     = "/dev/ttyUSB0"  # adapter si besoin
BAUDRATE = 115200            # doit correspondre au STM32

def list_ports():
    """Affiche tous les ports disponibles"""
    ports = serial.tools.list_ports.comports()
    if not ports:
        print("Aucun port trouvé")
        return
    print("Ports disponibles :")
    for p in ports:
        print(f"  {p.device} — {p.description}")

def main():
    # Afficher les ports disponibles avant de se connecter
    list_ports()
    print()

    try:
        ser = serial.Serial(
            port     = PORT,
            baudrate = BAUDRATE,
            bytesize = serial.EIGHTBITS,
            parity   = serial.PARITY_NONE,
            stopbits = serial.STOPBITS_ONE,
            timeout  = 1
        )

        print(f"Connecté sur {PORT} à {BAUDRATE} baud")
        print("En attente de données... (Ctrl+C pour quitter)")
        print("-" * 40)
        while True:
            
            if ser.in_waiting > 0:
                raw  = ser.readline()
                text = raw.decode("utf-8", errors="replace").strip()
                if text:
                    print(f"{text}")
                    #je transforme le type str en list 
                    z=list(text)
                    #print(str(z[11:])) pour tester la conversion et extraire la liste converti 
                    #extraire la liste qui contient le mot codé, et le convertir vers une liste car je vais l'utiliser dans le fichier bash
                    l=""
                    for i in z[11:] :
                        l+=i 
                    #print(l) conversion réussit 

                        

                    


    except serial.SerialException as e:
        print(f"Erreur : {e}")
        print("Vérifie que le port est correct et que tu as les permissions")
        print("  → sudo usermod -a -G dialout $USER")
    except KeyboardInterrupt:
        print("\nArrêt")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("Port fermé")

if __name__ == "__main__":
    main()
