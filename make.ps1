# Devido ao suporte limitado de Makefiles no Windows, este script foi criado para simular o comportamento de um Makefile no Windows (MSVC).
param (
    [switch]$clean = $false,
    [switch]$release = $false,
    [switch]$run = $false
)
$CFLAGS = "/Wall /WX /Iexternal\yyjson\src /wd4820 /wd4296 /wd5045 /wd4267 /wd4242 /wd4244 /wd4456" # Mostrar todos os avisos e considerar avisos como erros (Wall e WX) | Incluir o diretório de cabeçalhos do yyjson | Desativar avisos de padding e de conversão implícita
$LDFLAGS = ""
$sourceDir = "src"
$outputDir = "build"
$target = "gestao_stocks.exe"

if ($clean) {
    if (Test-Path "build") {
        Remove-Item -Recurse -Force -Path "build"
    }
    return
}


if ($release) {
    $CFLAGS += " -O2 -MD /wd4711 /wd4710" # Otimizar para velocidade (O2) | Desativar avisos de função inline
    $LDFLAGS += ""
}

if (!(Test-Path "build")) {
    New-Item -ItemType Directory -Path "build"
}

# List of source files in yyjson\src
$yyjson_src_files = Get-ChildItem -Path "external\yyjson\src" -Recurse -Include "*.c"

# List of source files in your project (excluding yyjson\src)
$project_src_files = Get-ChildItem -Path $sourceDir -Recurse -Include "*.c" 

# Concatenate the two lists
$c_files = @($yyjson_src_files) + @($project_src_files)

foreach ($file in $c_files) {
    # Get the relative path of the file
    $relativePath = $file.FullName -replace [regex]::Escape($PWD.Path) + '\\' 
    # Replace the source directory with the output directory
    $outputPath = $relativePath -replace [regex]::Escape($sourceDir), $outputDir
    # Replace the .c extension with .obj
    $outputPath = $outputPath -replace '\.c$', '.obj'
    # Remove the external\yyjson prefix
    $outputPath = $outputPath -replace [regex]::Escape('external\yyjson\'), ''

    # Compile the file
    "cl.exe $CFLAGS $relativePath -c -Fo:$outputPath" | Invoke-Expression
    # Se o ultimo comando falhou, sair.
    if ($LASTEXITCODE -ne 0) {
        exit
    }
}

# Link the object files
cl.exe $LDFLAGS $outputDir\*.obj -link -out:$outputDir\$target

if ($run -and $LASTEXITCODE -eq 0) {
    & ".\$outputDir\$target"
}

