cd source\code
for %%e in (cpp hpp h h.in c inl) do (
    for /R %%f in (*.%%e) do clang-format.exe -i -style=file %%f
)
cd ..\..