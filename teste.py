import urllib.parse

antigo = "media/eventos/21.°_Passeio_TT_Alguber_(4_rodas)/537/21.°_Passeio_TT_Alguber_(4_rodas)_537_c4dd6dbe-6040-48ad-9525-4ac4c4094166.zip"
novo = "https://pauloministro.com:8000/"


print(novo + urllib.parse.quote(antigo, safe=''))