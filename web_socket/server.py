from aiohttp import web
import socketio
import json
import jxmlease

# creates a new Async Socket IO Server
sio = socketio.AsyncServer()
# Creates a new Aiohttp Web Application
app = web.Application()
# Binds our Socket.IO server to our Web App
# instance
sio.attach(app)


# Seta o HTML no server
async def index(request):
    with open('index.html') as f:
        return web.Response(text=f.read(), content_type='text/html')


# If we wanted to create a new websocket endpoint,
# use this decorator, passing in the name of the
# event we wish to listen out for
@sio.on('message')
async def print_message(sid, message):
    # dumps na mensagem pra ter json perfeitamente em str
    message = json.dumps(message)
    print("Ta vindo: " , message)
    
    # Parseia o que vem do javascript
    obj = json.loads((message))

    folder = "datafolder/"
    if (obj["type"] == "1"):
        # Gera arquivo JSON
        f = open(folder+"data.json", "w")
        f.write(json.dumps(obj["noc"]))
        f.close()
        print("Gerei o arquivo JSON!")
    elif (obj["type"] == "2"):
        # Gera arquivo XML
        dataXML = jxmlease.emit_xml(obj["noc"])
        f = open(folder+"data.xml", "w")
        f.write(dataXML)
        f.close()
        print("Gerei o arquivo XML!")

# Adiciona um get pra '/'
app.router.add_get('/', index)

# We kick off our server
if __name__ == '__main__':
    web.run_app(app)