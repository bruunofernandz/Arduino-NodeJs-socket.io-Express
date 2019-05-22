const express = require('express')
const socketio = require('socket.io')
const http = require('http')
const serialport = require('serialport')

const app = express()
const server = http.createServer(app)

app.use(express.static('public')) //ADICIONA ARQUIVOS ESTATICOS

server.listen(3000, () => {
    console.log('Servidor HTTP Executando na porta: %d', server.address().port)
})

const io = socketio.listen(server)

//CONFIG SERIAL
const Readline = serialport.parsers.Readline
const parser = new Readline({ delimiter: '\r\n' }) //DECODIFICA HEXADECIMAL EM INT, STRING...
const mySerial = new serialport("/dev/ttyUSB0", {
    baudRate: 4800,
})

mySerial.pipe(parser)


mySerial.on('open', function() {
    console.log("CONEXAO SERIAL INICIADA")
    parser.on('data', data => {
        console.log(data)
        app.get('/', (req, res) => {
            res.writeHead(200, {'Content-Type':'Text/Html'}) //MINHA PAGINA DA WEB
            res.write(`
            <!DOCTYPE html>
            <html>
                <head>
                    <title>Title</title>
                    <meta charset="utf-8">
                    <meta name="viewport" content="width=device-width, initial-scale=1">
                    <!-- UIkit CSS -->
                    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/uikit/3.1.5/css/uikit.min.css" />
            
                    <!-- UIkit JS -->
                    <script src="https://cdnjs.cloudflare.com/ajax/libs/uikit/3.1.5/js/uikit.min.js"></script>
                    <script src="https://cdnjs.cloudflare.com/ajax/libs/uikit/3.1.5/js/uikit-icons.min.js"></script>
                    <script src="https://cdn.jsdelivr.net/npm/chart.js@2.8.0/dist/Chart.min.js"></script>
                    <script src="https://cdnjs.cloudflare.com/ajax/libs/socket.io/2.2.0/socket.io.js"></script>
                    <style>
                        iframe{
                            margin-top: 30px;
                        }
                    </style>
            
                </head>
                <body>            
                    <div class="uk-container uk-flex container" style="height:100vh; justify-content: center; align-items: center; flex-direction: column;">
                        <a href="${data}" target="_blank"><button class="uk-button uk-button-primary uk-border-rounded">TRACKER NOW!</button></a>
                        <iframe src="https://www.google.com/maps/embed?pb=!1m18!1m12!1m3!1d29517.734343731998!2d-47.38694030751995!3d-22.364319984445196!2m3!1f0!2f0!3f0!3m2!1i1024!2i768!4f13.1!3m3!1m2!1s0x94c87057a4792fa3%3A0x2bf2ad63b481c2fe!2sAraras+-+SP!5e0!3m2!1spt-BR!2sbr!4v1558558007850!5m2!1spt-BR!2sbr" width="600" height="450" frameborder="0" style="border:0" allowfullscreen></iframe>
                    </div>
                    
                </body>
            </html>
    
            `)
            res.end()
            //res.sendFile(__dirname + '/public/index.html')
        })
    })
})