Write-Host 'Fix Colores Secuencias de Escape Consola'
Write-Host ''
Write-Host 'Este script agregara el siguiente valor a la clave "\HKEY_CURRENT_USER\Console"'
Write-Host 'Nombre: VirtualTerminalLevel'
Write-Host 'Tipo: REG_DWORD'
Write-Host 'Valor: 0x00000001'
Write-Host ''

$input = read-host "Escriba y para confirmar el cambio | n para cancelar | u para deshacer los cambios"
if ($input -eq 'y'){
reg add HKEY_CURRENT_USER\Console /v VirtualTerminalLevel /t REG_DWORD /d 0x00000001 /f
Write-Output "El valor ha sido agregado!"
}elseif($input -eq 'u'){
reg add HKEY_CURRENT_USER\Console /v VirtualTerminalLevel /t REG_DWORD /d 0x00000000 /f
Write-Output "El valor ha sido eliminado!"
} else {
Write-Output "Cancelado."
}
