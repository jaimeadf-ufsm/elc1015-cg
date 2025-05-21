const paths = document.querySelectorAll('path');

let out = "VectorGraphic({"

for (const path of paths)
{
    let t = ""

    t += `\n    VectorFeature()`

    t += `\n        .WithPaths("${path.attributes.d.value}")`

    if (path.attributes.fill && path.attributes.fill !== 'none')
    {
        t += `\n        .WithFill(ColorRGB(0x${path.attributes.fill.value.replace('#', '')}))`
    }

    t += `,`

    out += t
}

out += "\n}).Materialize(),"